#include "draw.h"

void draw_bitmap(u8 *scr, const u8 *bitmap, u8 bg, u8 fg)
{
	for (u8 i = 0; i < 8; i++)
	{
		u8 row = *bitmap++;
		for (u8 j = 0; j < 8; j++)
		{
			*scr = bg;
			if (row & 1) *scr = fg;
			scr++;
			row >>= 1;
		}
		scr += w - 8;
	}
}

void draw_bitmap_transparent(u8 *scr, const u8 *bitmap, u8 fg)
{
	for (u8 i = 0; i < 8; i++)
	{
		u8 row = *bitmap++;
		for (u8 j = 0; j < 8; j++)
		{
			if (row & 1) *scr = fg;
			scr++;
			row >>= 1;
		}
		scr += w - 8;
	}
}

void put_pixel_dither(u8 *const scr, u16 x, u16 y, u8 r, u8 g, u8 b)
{
	if (x >= w || y >= h) return;
	u8 finalr, finalg, finalb;
	u8 pr = 0, pg = 0, pb = 0;
	if (r == 0) { finalr = 0; pr = 1; }
	else if (r == 255) { finalr = 5; pr = 1; }
	if (g == 0) { finalg = 0; pg = 1; }
	else if (g == 255) { finalg = 5; pg = 1; }
	if (b == 0) { finalb = 0; pb = 1; }
	else if (b == 255) { finalb = 5; pb = 1; }
	float rf = (float)r * 0.0196078431372549f;
	float gf = (float)g * 0.0196078431372549f;
	float bf = (float)b * 0.0196078431372549f;
	float rnd = randf();
	if (pr == 0)
	{
		float leftover = rf - (u32)rf;
		u8 inc = (u8)(rnd + leftover);
		finalr = (u8)rf + inc;
	}
	if (pg == 0)
	{
		float leftover = gf - (u32)gf;
		u8 inc = (u8)(rnd + leftover);
		finalg = (u8)gf + inc;
	}
	if (pb == 0)
	{
		float leftover = bf - (u32)bf;
		u8 inc = (u8)(rnd + leftover);
		finalb = (u8)bf + inc;
	}
	u8 col = finalb * 36 + finalg * 6 + finalr;
	scr[y*w+x] = col;
}

void put_pixel(u8 *const scr, u16 x, u16 y, u8 col)
{
	if (x >= w || y >= h) return;
	scr[y*w+x] = col;
}
