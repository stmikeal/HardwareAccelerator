#pragma once

#include "types.h"

#define io_buf_points                                                           \
    (*(volatile Coords(*)[IO_BUF_INT_LENGTH / (sizeof(Coords) / sizeof(int))])( \
        IO_BUF_ADDR))

#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)

unsigned int abs(int a) { return (unsigned int)((a > 0) ? a : -a); }

unsigned int sqrt(unsigned int n)
{
    if (n == 0)
        return 0;
    if (n < 4)
        return 1;

    unsigned int x = n / 2;
    while (1)
    {
        unsigned int y = (x + n / x) / 2;
        if (y >= x)
        {
            return x;
        }
        x = y;
    }
}

#define IO_X (*(volatile unsigned int *)(0x80000010))
#define IO_Y (*(volatile unsigned int *)(0x80000014))
#define IO_RES (*(volatile unsigned int *)(0x80000018))

inline unsigned int custom0_instr_wrapper(unsigned int a, unsigned int b)
{
    unsigned int result;
    asm volatile(".insn r 0x0b, 0x0, 0x0, %0, %1, %2"
                 : "=r"(result)
                 : "r"(a), "r"(b));
    return result;
}
