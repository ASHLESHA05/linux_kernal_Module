# kernel-module-process-tree

**A Linux Kernel Module for Process Tree Interaction**

This repository houses a Linux kernel module designed to create a character device named "kernel," offering the following functionalities:

- **Exclusive Device Access:** Ensures only one process can open the device at a time, safeguarding data integrity and preventing conflicts.
- **Process Tree Inspection:** Upon opening the device, it presents a clear overview of the current process tree, providing valuable insights into system activity.
- **Data Storage:** Facilitates both writing and reading arbitrary data to and from the device, enabling potential use cases in data exchange and configuration management.

## Key Features

- **Semaphore-Based Synchronization:** Employs a semaphore to coordinate exclusive access, ensuring consistent and reliable operations.
- **Process Tree Output:** Offers a visual representation of process relationships, aiding in system analysis and troubleshooting.
- **Customizable Data Storage:** Supports both reading and writing functions for flexible data management.

## Development and Usage

### Requirements

- Linux kernel development environment with appropriate headers
- Root privileges for module loading and device node creation

### Building the Module

1. Navigate to the source code directory (containing a Makefile).
2. Execute `make` to compile the module.

### Loading the Module

1. Use `insmod kernel.ko` as root to insert the module into the kernel.
2. Inspect the assigned major number using `dmesg | grep major`.

### Interacting with the Device

1. Create the device node using `mknod /dev/kernel c <major_number> 0`, substituting `<major_number>` with the actual value from dmesg.
2. Employ standard tools like `cat`, `echo`, and `dd` for interaction:
    - Open (and print process tree): `cat /dev/kernel`
    - Write data: `echo "Hello" > /dev/kernel`
    - Read data: `dd if=/dev/kernel bs=1 count=100`

### Unloading the Module

1. Unload using `rmmod kernel` as root.

## Important Notes

- **Educational Purposes:** This module serves primarily as a learning resource for kernel module development.
- **Stability:** Exercise caution when loading and unloading kernel modules to avoid potential system instability.

## Further Information

- Kernel module development documentation: [https://docs.kernel.org/](https://docs.kernel.org/)

**License:** GPL
