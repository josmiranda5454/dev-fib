#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>

MODULE_LICENSE("GPL");
#define DEV_FIBONACCI_NAME "fibonacci"

// Global static variables to be used throughout
static dev_t fib_dev = 0;
static struct cdev *fib_cdev;
static struct class *fib_class;

// static functions that will be used by the file_operations structure
static ssize_t fib_read(struct file *file, char __user *user_buffer, size_t size, loff_t *offset)
{
    return 0;
}

const struct file_operations fib_fops = {
    .owner = THIS_MODULE,
    .read = fib_read,
};

static int __init init_fib_dev(void) {
    int rc = 0;

    printk(KERN_INFO "Initializing the fibonacci device");

    // Let's register the device
    // This will dynamically allocate the major number
    rc = alloc_chrdev_region(&fib_dev, 0, 1, DEV_FIBONACCI_NAME);

    if (rc < 0) {
        printk(KERN_ALERT "Failed to register the fibonacci char device. rc = %i", rc);
        return -1;
    }

    fib_cdev = cdev_alloc();
    if (fib_cdev == NULL) {
        printk(KERN_ALERT "Failed to alloc cdev");
        unregister_chrdev_region(fib_dev, 1);
        return -1;
    }
    cdev_init(fib_cdev, &fib_fops);

    rc = cdev_add(fib_cdev, fib_dev, 1);
    if (rc < 0) {
        printk(KERN_ALERT "Failed to alloc cdev");
        unregister_chrdev_region(fib_dev, 1);
        return -1;
    }

    fib_class = class_create(THIS_MODULE, DEV_FIBONACCI_NAME);

    if (!fib_class) {
        printk(KERN_ALERT "Failed to alloc cdev");
        cdev_del(fib_cdev);
        unregister_chrdev_region(fib_dev, 1);
        return -1;
    }

    if (!device_create(fib_class, NULL, fib_dev, NULL, DEV_FIBONACCI_NAME)) {
        printk(KERN_ALERT "Failed to alloc cdev");
        class_destroy(fib_class);
        cdev_del(fib_cdev);
        unregister_chrdev_region(fib_dev, 1);
        return -1;
    }

    return 0;
}

static void __exit exit_fib_dev(void) {
    printk(KERN_INFO "Exiting and releasing the fibonnaci device");
    device_destroy(fib_class, fib_dev);
    class_destroy(fib_class);
    cdev_del(fib_cdev);
    unregister_chrdev_region(fib_dev, 1);
}

// Entry and Exit point for 
// the driver
module_init(init_fib_dev);
module_exit(exit_fib_dev);
