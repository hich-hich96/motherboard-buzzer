#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <unistd.h>
#include "inpout32.h"

// types for functions from library, since I'm using runtime library loading
// You could link that library instead of that
typedef int(_stdcall *f_inp32_t)(short);
typedef int(_stdcall *f_out32_t)(short, short);
typedef int(_stdcall *f_is32_t)();
f_out32_t out32;
f_inp32_t inp32;
f_is32_t is64;

void start_beep(short frequency)
{
    out32(0x43, 0xB6);
    int div = 0x1234dc / frequency;
    out32(0x42, div & 0xFF);
    out32(0x42, (div >> 8) & 0xFF);
    out32(0x61, inp32(0x61) | 0x03);
}

void stop_beep()
{
    out32(0x61, inp32(0x61) & 0xFC);
}

int main(void)
{
    // DLL Loading
    HINSTANCE beep_dll = LoadLibrary("inpoutx64.dll");
    if (!beep_dll)
        return -1;

    out32 = (f_out32_t)GetProcAddress(beep_dll, "Out32");
    inp32 = (f_inp32_t)GetProcAddress(beep_dll, "Inp32");
    is64 = (f_is32_t)GetProcAddress(beep_dll, "IsXP64Bit");
    // END OF DLL Loading

    printf("Start beeping ..%d\n", is64());
    for (int i = 0; i < 10; i++)
    {
        start_beep(1000);
        usleep(1000000); // 1 second
        stop_beep();
        usleep(1000000); // 1 second
    }
    return 0;
}
