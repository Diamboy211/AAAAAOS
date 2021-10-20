#include "aaaaaint.h"
#include "math.h"

static inline u32 as_u32(float x)
{
	return *(u32 *)&x;
}

static inline s32 as_s32(float x)
{
	return *(s32 *)&x;
}

static inline float as_float(u32 x)
{
	return *(float *)&x;
}

float fneg(float a)
{
	return as_float(as_u32(a) ^ 0x80000000);
}

static const float sd = 1.0f / 8388608.0f;
static const float su = 8388608.0f;
static const u32 onei = 0x3F800000;

float flog2(float a)
{
	return (float)(as_s32(a) - onei) * sd;
}

float fexp2(float a)
{
	return as_float((s32)(a * su) + onei);
}

float fsqrt(float a)
{
	float y = as_float((as_u32(a) >> 1) + (onei >> 1));
	return y - (y*y-a) / (2.0f*y);
}

float finvsqrt(float a)
{
	asm volatile (
		"\trsqrtss (%0), %%xmm0\n"
		"\tmovss %%xmm0, (%0)\n"
	:
	: "r" (&a)
	: "xmm0"
	);
	return a;
}