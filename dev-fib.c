#include <linux/init.h>
#include <linux/module.h>


static int init_fib_dev(void) {
    printk(KERN_ALERT "Initializing the fibonacci device");
    return 0;
}

static void exit_fib_dev(void) {
    printk(KERN_ALERT "Exiting and releasing the fibonnaci device");
}



// Entry and Exit point for 
// the driver
module_init(init_fib_dev);
module_exit(exit_fib_dev);
