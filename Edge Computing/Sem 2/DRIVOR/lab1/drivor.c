#include <linux/module.h>
#include <linux/init.h>

static int __init my_init(void) {
	return 0;
}
static void __exit my_exit(void) {
}

module_init(my_init);
module_exit(my_exit);
MODULE_AUTHOR("REDACTED");
MODULE_DESCRIPTION("");
MODULE_LICENSE("GPL");