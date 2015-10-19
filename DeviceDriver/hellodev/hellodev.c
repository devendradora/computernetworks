#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("DORA LICENSE");

static int hello_init(void) {
  printk(KERN_INFO "Hello dev!\n");
  return 0;
}

static void hello_exit(void) {
  printk("<1> Bye\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Devendra Dora");
MODULE_DESCRIPTION("USB  Driver");
