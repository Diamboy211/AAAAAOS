#include "aaaaaint.h"
// #include "font.h"
#include "io.h"
#include "rand.h"
#include "idt.h"
#include "draw.h"
#include "constant.h"
#include "math.h"

u8 popcnt64(u64 x)
{
	u8 c = 0;
	for (; x != 0; x &= x - 1)
		c++;
	return c;
}

void waste_cycles(u32 duration)
{
	asm volatile (
	"1:\n\t"
		"dec %%edx\n\t"
		"jnz 1b\n"
	:
	: "d" (duration)
	:
	);
}

void cmplx_mult(double *out, double *a, double *b)
{
	out[0] = a[0]*b[0] - a[1]*b[1];
	out[1] = a[1]*b[0] + a[0]*b[1];
}

void cmplx_div(double *out, double *a, double *b)
{
	double prod = 1.0 / (b[0]*b[0]+b[1]*b[1]);
	out[0] = (a[0]*b[0] + a[1]*b[1]) * prod;
	out[1] = (a[1]*b[0] - a[0]*b[1]) * prod;
}

void func_find_root(double *out, double *x)
{
	cmplx_mult(out, x, x);
	out[0] -= 1.0f;
}

void func_find_rootd(double *out, double *x)
{
	out[0] = x[0] + x[0];
	out[1] = x[1] + x[1];
}

extern void kernel_main()
{
	idt_init();
	u8 *const back_scr = (u8 *)0x100000;
	// u32 d = 1193180 / 440;
	// outb(0x42, (u8)(d));
	// outb(0x42, (u8)(d >> 8));
	// u8 tmp = inb(0x61);
	// outb(0x61, tmp | 3);

	outb(0x03C8, 0);
	for (int i = 0; i < 216; i++)
	{
		float r = i % 6;
		float g = (i / 6) % 6;
		float b = i / 36;
		r *= 63.0f/5.0f;
		g *= 63.0f/5.0f;
		b *= 63.0f/5.0f;
		outb(0x03C9, (u8)r);
		outb(0x03C9, (u8)g);
		outb(0x03C9, (u8)b);
	}
	for (int i = 216; i < 256; i++)
	{
		u8 c = (float)(i - 216) / 39.0f * 63.0f;
		outb(0x03C9, c);
		outb(0x03C9, c);
		outb(0x03C9, c);
	}
	{
		u8 *tmp = back_scr;
		while (tmp < back_scr+w*h)
		{
			u32 pixel_pos = tmp - back_scr;
			u16 p[2] = {pixel_pos % w, pixel_pos / w};
			float x = (float)(p[0] - w/2) / ((float)w * 2.0f);
			float y = x;
			flog2(&y);
			u16 ypos = (y + 1.0f) * (float)h * 0.5f;
			if (p[1] == ypos) *tmp++ = 0;
			else *tmp++ = 215;
		}
		for (int i = 0; i < w*h; i++) screen[i] = back_scr[i];
	}

	for (;;)
	{
		asm("hlt");
	}
}
