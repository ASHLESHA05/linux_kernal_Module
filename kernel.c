
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/semaphore.h>
#include <asm/uaccess.h>
#include <linux/sched.h>

// Structure to represent the fake device
MODULE_LICENSE("GPL");

struct fake_device {
    char data[100];
    struct semaphore sem;
} virtual_device;

// Variable to store the device's cdev structure and major number
struct cdev *mcdev;
int major_number;
dev_t dev_num;

#define DEVICE_NAME "kernal"
// Called when a process tries to open the device file
static void print_process_tree(struct task_struct *task, int level) {
    struct task_struct *child_task;
    struct list_head *list;

    // Print task information
    printk(KERN_INFO "%*s- Name: %s, PID: %d\n", level * 2, "", task->comm, task->pid);

    // Print children
    list_for_each(list, &task->children) {
        child_task = list_entry(list, struct task_struct, sibling);
        print_process_tree(child_task, level + 1);
    }
}
int device_open(struct inode *inode, struct file *filp) {
    // Allow only one process to open this device by using a semaphore
    if (down_interruptible(&virtual_device.sem) != 0) {
        printk(KERN_ALERT "Failed to lock the device\n");
        return -EBUSY; // Device is busy
    }
    printk(KERN_INFO "Device opened\n");

    // Print process tree
    printk(KERN_INFO "Process Tree:\n");
    print_process_tree(&init_task, 0);

    return 0;
}



// Called when a process tries to read from the device
ssize_t device_read(struct file *filp, char __user *buf, size_t len, loff_t *offset) {
    ssize_t ret = 0;
    // Check if buffer size is valid
    if (len < sizeof(virtual_device.data)) {
        return -EINVAL; // Invalid argument
    }
    // Copy device data to user buffer
    if (copy_to_user(buf, virtual_device.data, sizeof(virtual_device.data)) != 0) {
        ret = -EFAULT; // Bad address
    }
    printk(KERN_INFO "Data read from device\n");
    return ret;
}

// Called when a process tries to write to the device
ssize_t device_write(struct file *filp, const char __user *buf, size_t len, loff_t *offset) {
    ssize_t ret = 0;
    // Check if buffer size is valid
    if (len > sizeof(virtual_device.data)) {
        return -EINVAL; // Invalid argument
    }
    // Copy user buffer to device data
    if (copy_from_user(virtual_device.data, buf, len) != 0) {
        ret = -EFAULT; // Bad address
    }
    // Print the data written by the user
    printk(KERN_INFO "Data written to device: %s\n", virtual_device.data);
    return ret;
}

// Called when a process closes the device file
int device_close(struct inode *inode, struct file *filp) {
    // Release the semaphore obtained at device open
    up(&virtual_device.sem);
    printk(KERN_INFO "Device closed\n");
    return 0;
}

// File operations structure containing pointers to the device's operations
struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = device_open,
    .release = device_close,
    .write = device_write,
    .read = device_read
};

// Module initialization function
static int __init driver_entry(void) {
    // Allocate a major number dynamically for the device
    int ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
    if (ret < 0) {
        printk(KERN_ALERT "Failed to allocate a major number\n");
        return ret;
    }
    major_number = MAJOR(dev_num);
    printk(KERN_INFO "Major number is: %d\n", major_number);

    // Initialize the semaphore
    sema_init(&virtual_device.sem, 1);

    // Allocate and initialize a cdev structure
    mcdev = cdev_alloc();
    mcdev->ops = &fops;
    mcdev->owner = THIS_MODULE;

    // Add the cdev structure to the kernel
    ret = cdev_add(mcdev, dev_num, 1);
    if (ret < 0) {
        printk(KERN_ALERT "Failed to add the device to the kernel\n");
        return ret;
    }

    printk(KERN_INFO "Device registered\n");
    return 0;
}

// Module exit function
static void __exit driver_exit(void) {
    // Remove the cdev structure from the kernel
    cdev_del(mcdev);
    // Unregister the device number
    unregister_chrdev_region(dev_num, 1);
    printk(KERN_ALERT "Unloaded module\n");
}

// Inform the kernel about the module's entry and exit points
module_init(driver_entry);
module_exit(driver_exit);
