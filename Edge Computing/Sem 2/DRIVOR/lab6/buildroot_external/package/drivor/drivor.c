#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>

#define FIRST_DEVICE_ADDR 0x01000000

static void __iomem* ioBase = NULL;

static const struct of_device_id deviceSearch[] = 
{
    { .compatible = "jakopf,first_device" },
    { }
};
MODULE_DEVICE_TABLE(of, deviceSearch);

static ssize_t readKObjectAttribute(struct kobject* kobj, struct kobj_attribute* attr, char* buf)
{
	int val = 0;

	pr_debug("[readKObjectAttribute] Trying to read from a physical register \"%s\"\n", attr->attr.name);

	if (!strcmp(attr->attr.name, "arg1"))
		memcpy_fromio(&val, ioBase, 4);	
	else if (!strcmp(attr->attr.name, "arg2"))
		memcpy_fromio(&val, ioBase + 0x4, 4);	
	else if (!strcmp(attr->attr.name, "operation"))
		memcpy_fromio(&val, ioBase + 0x8, 4);	
	else if (!strcmp(attr->attr.name, "result"))
		memcpy_fromio(&val, ioBase + 0xC, 4);
	else 
	{
		pr_info("[readKObjectAttribute] Couldn't find Attribute \"%s\"\n", attr->attr.name);
		return -22;	
	}
	pr_info("[readKObjectAttribute] Successful read \"%s\" from \"/sysfs/kernel/%s\" with value: %d\n", attr->attr.name, attr->attr.name, val);

	return sprintf(buf, "%d\n", val);
}

static ssize_t writeKObjectAttribute(struct kobject* kobj, struct kobj_attribute* attr, const char* buf, size_t count)
{
	int val;
	sscanf(buf, "%d", &val);

	pr_info("[writeKObjectAttribute] Trying to write to a physical register \"%s\"\n", attr->attr.name);
	if (!strcmp(attr->attr.name, "arg1"))
		memcpy_toio(ioBase, &val, 4);
	else if (!strcmp(attr->attr.name, "arg2"))
		memcpy_toio(ioBase + 0x4, &val, 4);
	else if (!strcmp(attr->attr.name, "operation"))
		memcpy_toio(ioBase + 0x8, buf, 1);
	else 
	{
		pr_info("[writeKObjectAttribute] Couldn't write to Attribute \"%s\"\n", attr->attr.name);
		return -22;	
	}
	pr_info("[writeKObjectAttribute] Successful wrote to \"%s\" in \"/sysfs/kernel/%s\" with value: %d\n", attr->attr.name, attr->attr.name, (strcmp(attr->attr.name, "operation") ? val : buf[0]));

	return count;
}

static struct kobject* firstDeviceKObject;
static struct kobj_attribute arg1Attribute = __ATTR(arg1,      0660, readKObjectAttribute, writeKObjectAttribute);
static struct kobj_attribute arg2Attribute = __ATTR(arg2,      0660, readKObjectAttribute, writeKObjectAttribute);
static struct kobj_attribute opAttribute   = __ATTR(operation, 0660, readKObjectAttribute, writeKObjectAttribute);
static struct kobj_attribute resAttribute  = __ATTR(result,    0660, readKObjectAttribute, NULL);

static void spawnTestScript(void) //voodoo magic
{
	const char* scriptPath = "/test.sh";
	char script[512] = {0};
    	snprintf(script, 512, "#!/bin/sh\n"
		"echo 4 > /sys/kernel/first_device/arg1\n"
		"echo 6 > /sys/kernel/first_device/arg2\n"
		"echo * > /sys/kernel/first_device/operation\n"
		"cat /sys/kernel/first_device/result");

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
	if (platform_driver_register(&deviceDriver))
	{
		pr_crit("Failed to register device driver \"first_device\"\n");
		return -7;
	}
	
	ioBase = ioremap(FIRST_DEVICE_ADDR, 0x10);
	
	if (ioBase == NULL)
	{
		platform_driver_unregister(&deviceDriver);
		pr_crit("Could not map io memory!\n");
		return -9;
	}

	firstDeviceKObject = kobject_create_and_add("first_device", kernel_kobj);
	if (!firstDeviceKObject)
	{
		pr_crit("Could not create a KObject!\n");
		return -12;
	}

	if (sysfs_create_file(firstDeviceKObject, &arg1Attribute.attr))
	{
		pr_crit("Could not create a KObject Attribute \"arg1\"!\n");
		return -13;
	}

	if (sysfs_create_file(firstDeviceKObject, &arg2Attribute.attr))
	{
		pr_crit("Could not create a KObject Attribute \"arg2\"!\n");
		return -13;
	}

	if (sysfs_create_file(firstDeviceKObject, &opAttribute.attr))
	{
		pr_crit("Could not create a KObject Attribute \"operation\"!\n");
		return -13;
	}

	if (sysfs_create_file(firstDeviceKObject, &resAttribute.attr))
	{
		pr_crit("Could not create a KObject Attribute \"result\"!\n");
		return -13;
	}

	pr_info("Added \"first_device\" successfully\n");

	spawnTestScript();	
	return 0;
}

static void __exit my_exit(void) 
{
	sysfs_remove_file(firstDeviceKObject, &arg1Attribute.attr);
	sysfs_remove_file(firstDeviceKObject, &arg2Attribute.attr);
	sysfs_remove_file(firstDeviceKObject, &opAttribute.attr);
	sysfs_remove_file(firstDeviceKObject, &resAttribute.attr);
	kobject_put(firstDeviceKObject);

	iounmap(ioBase);
	platform_driver_unregister(&deviceDriver);
	pr_info("Removed \"first_device\" successfully\n");
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("REDACTED");
MODULE_DESCRIPTION("");
MODULE_LICENSE("GPL");