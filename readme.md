# LearningKernel 
> The kernel is a computer program at the core of a computer's operating system that has complete control over everything in the system. It is the "portion of the operating system code that is always resident in memory", and facilitates interactions between hardware and software components. On most systems, the kernel is one of the first programs loaded on startup (after the bootloader). It handles the rest of startup as well as memory, peripherals, and input/output (I/O) requests from software, translating them into data-processing instructions for the central processing unit. 

![Alt text](https://raw.githubusercontent.com/sebiiV/learningKernel/master/images/demo.PNG "demo")


## Definitions

``IOCTL``   Input and Output controls\
``IRP``     I/O Request Packet  


## Tools

OSRLoader https://www.osronline.com/article.cfm%5Earticle=157.htm \
HackSysVulnerabledriver https://github.com/hacksysteam/HackSysExtremeVulnerableDriver.git \
Hyper-V \
winDBG





## Commands
Finding a loaded driver
```cmd
C:\Users\kernel>driverquery | FindStr HEVD
HEVD         HEVD                   Kernel        25/02/2021 21:55:53
```
Generating cyclical payload and finding it
```bash
kali@kali:~/Desktop$ /usr/share/metasploit-framework/tools/exploit/pattern_create.rb -l 4000
Aa0Aa1Aa2Aa3Aa4Aa5Aa6Aa7Aa8Aa9Ab0Ab1Ab2Ab3Ab4Ab5Ab6Ab7Ab8Ab9Ac0A ... 
kali@kali:~/Desktop$ /usr/share/metasploit-framework/tools/exploit/pattern_offset.rb -q 31724330
[*] Exact match at offset 2072

```

wingdb add symbols
```c
kd> .sympath+ C:\Users\sebiV\git\HackSysExtremeVulnerableDriver\build\driver\windows\vulnerable\x64
kd> .reload /f // ages! don' use /f unless needed
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


## Reverse engineering

Like DLLs, Drivers specify an entrypoint, called ``DriverEntry`` and has the following signature
```c
NTSTATUS DriverEntry(
  _In_ struct _DRIVER_OBJECT *DriverObject,
  _In_ PUNICODE_STRING       RegistryPath
)
```
Which initialises the driver
```c
  RtlInitUnicodeString(&DeviceName,L"\\Device\\HackSysExtremeVulnerableDriver");
  RtlInitUnicodeString(&DosDeviceName,L"\\DosDevices\\HackSysExtremeVulnerableDriver");
  uVar1 = IoCreateDevice(param_1,0,&DeviceName,0x22,0x100,0,&DeviceObject);
```

In order to see how each ``IOCTL`` request is handled we look at ``IrpDeviceIoCtlHandler``
```c
// which devolves into a huge if{}else{} chain
    ...
          if (uVar2 == 0x222003) {
            DbgPrintEx(0x4d,3,"****** HEVD_IOCTL_BUFFER_OVERFLOW_STACK ******\n");
            uVar4 = BufferOverflowStackIoctlHandler(param_2,lVar1);
            uVar2 = (uint)uVar4;
            pcVar6 = "****** HEVD_IOCTL_BUFFER_OVERFLOW_STACK ******\n";
          }
    ...
```


## winDBG

```c
kd> bp HEVD!TriggerStackOverflow
```



# Links
---

https://docs.microsoft.com/en-us/windows-hardware/drivers/debugger/setting-up-a-network-debugging-connection-automatically

https://www.samdb.xyz/2016/01/17/intro_to_kernel_exploitation_part_1.html

https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/windows-kernel-mode-i-o-manager?redirectedfrom=MSDN

https://docs.microsoft.com/en-us/windows-hardware/drivers/kernel/dma-programming-techniques?redirectedfrom=MSDN

