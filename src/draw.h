#ifndef DRAW_H
#define DRAW_H

#include "constant.h"
#include "aaaaaint.h"
#include "rand.h"
void draw_bitmap(u8 *scr, const u8 *bitmap, u8 bg, u8 fg);
void draw_bitmap_transparent(u8 *scr, const u8 *bitmap, u8 fg);
void put_pixel_dither(u8 *const scr, u16 x, u16 y, u8 r, u8 g, u8 b);
void put_pixel(u8 *const scr, u16 x, u16 y, u8 col);

#endif /* DRAW_H */
