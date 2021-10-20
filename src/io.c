#include "io.h"

void outb(u16 port, u8 value)
{
	asm volatile ("out %%al, %%dx" : : "a" (value), "d" (port) : "memory");
}

u8 inb(u16 port)
{
	u8 r;
	asm volatile ("in %%dx, %%al" : "=a" (r) : "d" (port) : "memory");
	return r;
}
