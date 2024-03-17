# Kernel Module: Process Tree Interaction

**A Linux Kernel Module for Process Tree Interaction**

This repository houses a Linux kernel module designed to create a character device named "kernel," providing comprehensive functionalities for inspecting the process tree and facilitating data exchange.

## Overview

The kernel module offers the following key features:

- **Exclusive Device Access:** Ensures only one process can open the device at a time, safeguarding data integrity and preventing conflicts.
- **Process Tree Inspection:** Provides a clear overview of the process tree based on the specified parameter, offering valuable insights into system activity.
- **Data Storage:** Enables both writing and reading arbitrary data to and from the device, facilitating potential use cases in data exchange and configuration management.

## Key Features

- **Semaphore-Based Synchronization:** Employs a semaphore mechanism to coordinate exclusive access, ensuring consistent and reliable operations.
- **Flexible Process Tree Output:** Offers a visual representation of process relationships, aiding in system analysis and troubleshooting.
- **Customizable Data Storage:** Supports both reading and writing functions for flexible data management.

## Development and Usage

### Requirements

- Linux kernel development environment with appropriate headers.
- Root privileges for module loading and device node creation.

### Building the Module

1. Navigate to the source code directory containing the Makefile.
2. Execute `make` to compile the module.

### Loading the Module

1. Use `insmod kernel.ko my_param_string=<param>` as root to insert the module into the kernel, replacing `<param>` with one of the following:
    - `"current"` to print the process tree starting from the current process.
    - A valid process ID (PID) to print the process tree starting from that PID.
    - Omitting the parameter will print the process tree starting from the init process.

2. Inspect the assigned major number using `dmesg | grep major`.

### Interacting with the Device

1. Create the device node using `mknod /dev/kernel c <major_number> 0`, substituting `<major_number>` with the actual value from `dmesg`.
2. Employ standard tools like `cat`, `echo`, and `dd` for interaction:
    - Open (and print process tree): `cat /dev/kernel`
    - Write data: `echo "Hello" > /dev/kernel`
    - Read data: `dd if=/dev/kernel bs=1 count=100`

### Unloading the Module



1. Unload using `rmmod kernel` as root.

### Images
1.current argument passed:
![image](https://github.com/ASHLESHA05/linux_kernal_Module/assets/115142220/b20e085d-979e-4003-94e5-e0096d2d3b50)

2.no arguments passed:
![image](https://github.com/ASHLESHA05/linux_kernal_Module/assets/115142220/c6ab1c01-1f21-4576-8560-c600147d0be0)




## Important Notes

- **Educational Purposes:** This module primarily serves as a learning resource for kernel module development.
- **Stability:** Exercise caution when loading and unloading kernel modules to avoid potential system instability.

## Further Information

- For more information on kernel module development, refer to the [Kernel documentation](https://docs.kernel.org/).

## License

This project is licensed under the terms of the GNU General Public License (GPL).
