#ifndef IDT_H
#define IDT_H

#include "aaaaaint.h"

extern void *isr_stub_table[];

typedef struct
{
	u16 isr_low;
	u16 kernel_cs;
	u8 reserved;
	u8 attributes;
	u16 isr_high;
} __attribute__((packed)) idt_entry_t;

typedef struct
{
	u16 limit;
	u32 base;
} __attribute__((packed)) idtr_t;

void idt_init(void);
#endif /* IDT_H */
