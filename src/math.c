#include "aaaaaint.h"
#include "math.h"

float *fneg(float *a)
{
	*(u32 *)a ^= 0x80000000;
	return a;
}

float *flog2(float *a)
{
	const u32 onei = 0x3F800000;
	const float s = 1.0f / 8388608.0f;
	*a = (*(u32 *)a - onei) * s;
	return a;
}