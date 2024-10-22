#include <linux/module.h>
#include <linux/init.h>

static int __init my_init(void)
{
	pr_emerg("init - emerg\n");
	pr_alert("init - alert\n");
	pr_crit("init - crit\n");
	pr_err("init - err\n");
	pr_warn("init - warn\n");
	pr_notice("init - notice\n");
	pr_info("init - info\n");
	printk(KERN_DEBUG"init - debug\n");
	printk(KERN_DEFAULT"init - default\n");	
	return 0;
}

static void __exit my_exit(void) 
{
	pr_emerg("exit - emerg\n");
	pr_alert("exit - alert\n");
	pr_crit("exit - crit\n");
	pr_err("exit - err\n");
	pr_warn("exit - warn\n");
	pr_notice("exit - notice\n");
	pr_info("exit - info\n");
	printk(KERN_DEBUG"exit - debug\n");
	printk(KERN_DEFAULT"exit - default\n");	
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("REDACTED");
MODULE_DESCRIPTION("");
MODULE_LICENSE("GPL");