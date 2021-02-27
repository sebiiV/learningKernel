# LearningKernel 
```
The kernel is a computer program at the core of a computer's operating system that has complete control over everything in the system. It is the "portion of the operating system code that is always resident in memory", and facilitates interactions between hardware and software components. On most systems, the kernel is one of the first programs loaded on startup (after the bootloader). It handles the rest of startup as well as memory, peripherals, and input/output (I/O) requests from software, translating them into data-processing instructions for the central processing unit. 
```

![alt text](images/demo.png "Bugcheck")

## Definitions

``IOCTL``   Input and Output controls\
``IRP``     I/O Request Packet  


## Tools

OSRLoader https://www.osronline.com/article.cfm%5Earticle=157.htm\
HackSysVulnerabledriver https://github.com/hacksysteam/HackSysExtremeVulnerableDriver.git \
Hyper-V \
winDBG





## Commands
```
C:\Users\kernel>driverquery | FindStr HEVD
HEVD         HEVD                   Kernel        25/02/2021 21:55:53
```


# Windows

### IOCTLs
You can interact with a driver from User Mode by making use of Input and Output Controls (IOCTLs), A driver defines which IOCTLs it supports by defining them using the CTL_CODE macro

- Driver Ids,Values under 0x800 are used by Microsoft and 0x800 or greater can be used by vendors, each function the driver supports has a different FunctionCode.

Example IOCTL
```cpp
// HEVD
#define IOCTL(Function) CTL_CODE(FILE_DEVICE_UNKNOWN, Function, METHOD_NEITHER, FILE_ANY_ACCESS)

#define HEVD_IOCTL_BUFFER_OVERFLOW_STACK                         IOCTL(0x800)

// https://github.com/microsoft/Windows-driver-samples/blob/master/general/ioctl/wdm/sys/sioctl.h
#define IOCTL_SIOCTL_METHOD_IN_DIRECT \
    CTL_CODE( SIOCTL_TYPE, 0x900, METHOD_IN_DIRECT, FILE_ANY_ACCESS  )

```

### General driver interaction

- Get a handle to  the device driver the ``CreateFile``
- Use ``DeviceIoControl`` to send an ``IRP`` packet
- The IRP is then passed down the Windows driver stack until a driver that can handle it is found
- pwn

## Links
---

https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/setting-up-a-network-debugging-connection-automatically

https://www.samdb.xyz/2016/01/17/intro_to_kernel_exploitation_part_1.html

https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/windows-kernel-mode-i-o-manager?redirectedfrom=MSDN

https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/dma-programming-techniques?redirectedfrom=MSDN

