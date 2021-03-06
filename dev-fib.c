#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/mutex.h>

MODULE_LICENSE("GPL");
#define DEV_FIBONACCI_NAME "fibonacci"
/* MAX_LENGTH is set to 92 because
 * ssize_t can't fit the number > 92
 */
#define MAX_LENGTH 92

// Global static variables to be used throughout journey
static dev_t fib_dev = 0;
static struct cdev *fib_cdev;
static struct class *fib_class;
static DEFINE_MUTEX(dev_fib_mutex);

static long long fib_sequence(long long k) {
    long long f[k+2];
    int i;

    f[0] = 0;
    f[1] = 1;

    for (i = 2; i <= k; i++) {
        f[i] = f[i-1] + f[i-2];
    }

    return f[k];
}
// static functions that will be used by the file_operations structure
static int fib_open(struct inode *inode, struct file *file) {
    if (!mutex_trylock(&dev_fib_mutex)){
        printk(KERN_ALERT "dev-fib is in use");
	return -EBUSY;
    }
    printk(KERN_INFO "Got an open, success!");
    return 0;
}
static int fib_release(struct inode *inode, struct file *file) {
    mutex_unlock(&dev_fib_mutex);
    printk(KERN_INFO "Got a release, success!");
    return 0;
}
static ssize_t fib_read(struct file *file, char *buf, size_t size, loff_t *offset)
{
    printk("Got a read, let's calculate the fibonacci number at that offset, %lld", *offset);
    return (ssize_t)fib_sequence(*offset);
}

static ssize_t fib_write(struct file *file, const char *buf, size_t size, loff_t *offset)
{
    printk(KERN_INFO "Got a write, but we are skipping");
    return 1;
}

// In order for the offset to work we need to actually
// implement it
// Taken from https://sites.google.com/site/linuxkernel88/sample-code/writing-a-character-driver
static loff_t fib_device_lseek(struct file *file, loff_t offset, int orig)
{
    loff_t new_pos=0;
    switch( orig ) {
        case 0: /* SEEK_SET: */
            new_pos = offset;
            break;
        case 1: /* SEEK_CUR: */
            new_pos = file->f_pos + offset;
            break;
        case 2: /* SEEK_END: */
            new_pos = MAX_LENGTH - offset;
            break;
        }
    // Check boundaries
    if( new_pos > MAX_LENGTH ) new_pos = MAX_LENGTH; // max case
    if( new_pos < 0 ) new_pos = 0; // min case
    file->f_pos = new_pos; // This is what we'll use now
    return new_pos;
}

const struct file_operations fib_fops = {
    .owner = THIS_MODULE,
    .read = fib_read,
    .write = fib_write,
    .open = fib_open,
    .release = fib_release,
    .llseek = fib_device_lseek,
};

static int __init init_fib_dev(void) {
    int rc = 0;

    mutex_init(&dev_fib_mutex);

    printk(KERN_INFO "Initializing the fibonacci device");

    // Let's register the device
    // This will dynamically allocate the major number
    rc = alloc_chrdev_region(&fib_dev, 0, 1, DEV_FIBONACCI_NAME);

    if (rc < 0) {
        printk(KERN_ALERT "Failed to register the fibonacci char device. rc = %i", rc);
        return rc;
    }

    fib_cdev = cdev_alloc();
    if (fib_cdev == NULL) {
        printk(KERN_ALERT "Failed to alloc cdev");
        rc = -1;
        goto failed_cdev;
    }
    cdev_init(fib_cdev, &fib_fops);
    rc = cdev_add(fib_cdev, fib_dev, 1);
 
    if (rc < 0) {
        printk(KERN_ALERT "Failed to add cdev");
        rc = -2;
        goto failed_cdev;
    }

    fib_class = class_create(THIS_MODULE, DEV_FIBONACCI_NAME);

    if (!fib_class) {
        printk(KERN_ALERT "Failed to create device class");
        rc = -3;
        goto failed_class_create;
    }

    if (!device_create(fib_class, NULL, fib_dev, NULL, DEV_FIBONACCI_NAME)) {
        printk(KERN_ALERT "Failed to create device");
        rc = -4;
        goto failed_device_create;
    }
    return rc;
failed_device_create:
    class_destroy(fib_class);
failed_class_create:
    cdev_del(fib_cdev);
failed_cdev:
    unregister_chrdev_region(fib_dev, 1);
    return rc;
}

static void __exit exit_fib_dev(void) {
    printk(KERN_INFO "Exiting and releasing the fibonnaci device");
    mutex_destroy(&dev_fib_mutex);
    device_destroy(fib_class, fib_dev);
    class_destroy(fib_class);
    cdev_del(fib_cdev);
    unregister_chrdev_region(fib_dev, 1);
}

// Entry and Exit point for 
// the driver
module_init(init_fib_dev);
module_exit(exit_fib_dev);
