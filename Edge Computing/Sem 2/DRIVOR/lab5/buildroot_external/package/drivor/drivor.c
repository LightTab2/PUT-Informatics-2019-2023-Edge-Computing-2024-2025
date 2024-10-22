#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/io.h>

#define BUFFER_SIZE 32
#define FIRST_DEVICE_ADDR 0x01000000

static char buffer[BUFFER_SIZE]        = {0};
static char bufferOut[BUFFER_SIZE + 1] = {0};
static void __iomem* ioBase            = NULL;
static int devicesCount                = 0;
static int bufferTextEndIndex          = 0;

static dev_t devID;
static struct cdev cDev;

struct class* firstDeviceClass = NULL;
struct device* dev = NULL;

static const struct of_device_id deviceSearch[] = 
{
    { .compatible = "jakopf,first_device" },
    { }
};

MODULE_DEVICE_TABLE(of, deviceSearch);

static int deviceOpen(struct inode* inodep, struct file* filep) 
{
	pr_info("[deviceOpen] Another file soldier to break, total count: %d\n", ++devicesCount); //Ta liczba powinna byc zawsze 0 lub 1 (dobrze rozumiem??). Jezeli nie, to program nie dziala poprawnie jak sie czyta 2 pliki na raz	
	return 0;
}

static int deviceRelease(struct inode* inodep, struct file* filep) 
{
	pr_info("[deviceRelease] One file soldier gone, total count: %d\n", --devicesCount);
	return 0;
}

static ssize_t deviceRead(struct file* filep, char* readBuffer, size_t len, loff_t* offset)
{
	if ((*offset + len) >= bufferTextEndIndex)
	{
		//pr_err("[deviceRead] Toooooo massive file to read, tried to read %lu bytes, when %lld are already read and the max is %d\nOutput will be limited to first %d chars\n", len, *offset, BUFFER_SIZE, BUFFER_SIZE);
		if ((bufferTextEndIndex - *offset) <= 0ll)
		{
			*offset += len;
			return 0;
		}
	}

	int readCount = 0; 
	memcpy_fromio(&readCount, ioBase, 4);	
	pr_info("[deviceRead] Successfuly read from a physical register \"readCount\": %d\n", readCount);

	++readCount;

	memcpy_toio(ioBase, &readCount, 4);
	pr_info("[deviceRead] Successfuly wrote to a physical register \"readCount\": %d\n", readCount);

	for (int i = 0; i != bufferTextEndIndex; ++i)
		bufferOut[i] = buffer[bufferTextEndIndex - i - 1];

	bufferOut[bufferTextEndIndex] = '\n';
	
	if (copy_to_user(readBuffer, bufferOut + *offset, bufferTextEndIndex + 1)) 
	{
		pr_err("[deviceRead] Failed during read\n");
		return -1;
	}

	*offset += len;

	pr_info("[deviceRead] Successfuly read this number of characters: %d\n", bufferTextEndIndex);

	int size = bufferTextEndIndex + 1;
	bufferTextEndIndex = 0;
	//BEZPIECZENSTWO, ZAWSZE WAZNE, DOBRA PRAKTYKA	
	memset(buffer,    0, BUFFER_SIZE);
	memset(bufferOut, 0, BUFFER_SIZE);

	return size;
}

static ssize_t deviceWrite(struct file* filep, const char* writeBuffer, size_t len, loff_t* offset) 
{
	long long size = len;
	if ((*offset + len) >= BUFFER_SIZE)
	{
		pr_err("[deviceWrite] Toooooo massive file to write, tried to write %lu bytes, when %lld are already written and the max is %d\n[deviceWrite] Input will be limited to first %d chars\n", len, *offset, BUFFER_SIZE, BUFFER_SIZE);
		size = BUFFER_SIZE - *offset;
		if (size <= 0ll)
		{
			*offset += len;
			return len;
		}
	}

	if (copy_from_user(buffer + *offset, writeBuffer, size))
    {
		pr_err("[deviceWrite] Failed during write\n");
		return -2;
   	}

	*offset += len;
	bufferTextEndIndex = size;

	pr_info("[deviceWrite] Successfuly wrote this number of characters: %lld\n", size);
	return size;
}

static struct file_operations fops = 
{
	.read = deviceRead,
	.write = deviceWrite,
	.open = deviceOpen,
	.release = deviceRelease
};

static void spawnTestScript(void) //voodoo magic
{
	const char* scriptPath = "/test.sh";
	char script[512] = {0};
    	snprintf(script, 512, "#!/bin/sh\n"
		//"mknod /dev/first_device c %d 0\n"
		"echo \"Krotki tekst\" > /dev/first_device\n"
		"cat /dev/first_device\n"
		"echo \"Dlugi tekst znacznie przekraczajacy bufor\" > /dev/first_device\n"
		"cat /dev/first_device\n"/*, MAJOR(devID)*/);

	loff_t pos = 0;

	struct file* filp = filp_open(scriptPath, O_WRONLY|O_CREAT|O_TRUNC, 0777);
	if (IS_ERR(filp))
	{
		pr_err("Failed to create test script \"%s\"\n", scriptPath);
		return;
	}

	if (kernel_write(filp, script, strlen(script), &pos) < 0)
		pr_err("Failed to create test script \"%s\"\n", scriptPath);
	else pr_info("Test script \"%s\" created successfully\n", scriptPath);
	
	filp_close(filp, NULL);
}

static int deviceProbe(struct platform_device* pdev)
{
	struct resource* res;
	void __iomem* baseAddr;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
	{
		dev_err(&pdev->dev, "[deviceRegister] No memory resource?.\n");
        	return -7;
   	}

	baseAddr = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(baseAddr))
	{
		dev_err(&pdev->dev, "[deviceRegister] Something wrong with memory allocation at 0x%08llx.\n", (unsigned long long)(baseAddr));
        return -8;
	}

	dev_info(&pdev->dev, "[deviceRegister] Device mapped: 0x%08llx:0x%08llx\n", (unsigned long long)(res->start), (unsigned long long)(res->end));

	const char* strVal  = "";
	unsigned    uintVal = 0u;
	bool        bVal    = false;

	if (!of_property_read_string(pdev->dev.of_node, "wisdom", &strVal))
		dev_info(&pdev->dev, "[deviceRegister] String Variable \"wisdom\": %s\n", strVal);
	else
		dev_err(&pdev->dev, "[deviceRegister] String Variable \"wisdom\" entry not found!\n");

	if (!of_property_read_u32(pdev->dev.of_node, "bestRegards", &uintVal))
		dev_info(&pdev->dev, "[deviceRegister] Int Variable \"bestRegards\": %d\n", uintVal);
	else
		dev_err(&pdev->dev, "[deviceRegister] Int Variable \"bestRegards\" entry not found!\n");

	bVal = of_property_read_bool(pdev->dev.of_node, "pi"); 
	dev_info(&pdev->dev, "[deviceRegister] Bool Variable \"pi\": %s\n", (bVal ? "true" : "false"));

	return 0;
}

static struct platform_driver deviceDriver = 
{
	.driver = 
	{
		.name = "first_device",
		.of_match_table = of_match_ptr(deviceSearch),
	},
	.probe = deviceProbe
};

static int __init my_init(void)
{
	if (alloc_chrdev_region(&devID, 0, 1, "first_device")) 
	{
		pr_crit("Could not allocate \"first_device\"\n");
        return -3;
    }
	pr_info("\"first_device\": major %d\n", MAJOR(devID));

	cdev_init(&cDev, &fops);
	if (cdev_add(&cDev, devID, 1)) 
	{
		unregister_chrdev_region(devID, 1);
		printk(KERN_ALERT "Failed to register cdev\n");
		return -4;
	}

	firstDeviceClass = class_create("first_device_class");
	if (IS_ERR(firstDeviceClass))
	{
		cdev_del(&cDev);
		unregister_chrdev_region(devID, 1);
		pr_crit("Failed to create class for \"first_device\"\n");
		return -5;
	}

	dev = device_create(firstDeviceClass, NULL, devID, NULL, "first_device");
	if (IS_ERR(dev)) 
	{
		class_destroy(firstDeviceClass);
		cdev_del(&cDev);
		unregister_chrdev_region(devID, 1);
		pr_crit("Failed to create device \"first_device\"\n");
		return -6;
	}

	if (platform_driver_register(&deviceDriver))
	{
		device_destroy(firstDeviceClass, devID);
		class_destroy(firstDeviceClass);
		cdev_del(&cDev);
		unregister_chrdev_region(devID, 1);
		pr_crit("Failed to register device driver \"first_device\"\n");
		return -7;
	}
	
	ioBase = ioremap(FIRST_DEVICE_ADDR, 4);
	
	if (ioBase == NULL)
	{
		platform_driver_unregister(&deviceDriver);
		device_destroy(firstDeviceClass, devID);
		class_destroy(firstDeviceClass);
		cdev_del(&cDev);
		unregister_chrdev_region(devID, 1);
		pr_crit("Could not map io memory!\n");
		return -9;
	}

	pr_info("Added \"first_device\" successfully\n");

	spawnTestScript();	
	return 0;
}

static void __exit my_exit(void) 
{
	iounmap(ioBase);
	platform_driver_unregister(&deviceDriver);
	device_destroy(firstDeviceClass, devID);
	class_destroy(firstDeviceClass);
	cdev_del(&cDev);
	unregister_chrdev_region(devID, 1);
	pr_info("Removed \"first_device\" successfully\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("REDACTED");
MODULE_DESCRIPTION("");
MODULE_LICENSE("GPL");