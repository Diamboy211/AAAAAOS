#include "idt.h"

__attribute((aligned(0x10)))
static idt_entry_t idt[256];
static idtr_t idtr;

void idt_set_desc(u8 vector, void *isr, u8 flags)
{
	idt_entry_t *desc = idt + vector;
	desc->isr_low = (u32)isr & 0xFFFF;
	desc->kernel_cs = 0x08;
	desc->attributes = flags;
	desc->isr_high = (u32)isr >> 16;
	desc->reserved = 0;
}

void idt_init()
{
	idtr.base = (u16)(u32)idt;
	idtr.limit = (u16)sizeof(idt_entry_t) * 32 - 1;

	for (u8 vector = 0; vector < 32; vector++)
	{
		idt_set_desc(vector, isr_stub_table[vector], 0x8E);
	}
	asm volatile("lidt %0; sti" : : "m"(idtr));
}

