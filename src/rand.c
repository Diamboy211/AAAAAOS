#include "rand.h"

u64 state0 = 0xdeadbeef;
u64 state1 = 0xc0d1e2f3;

u64 rand64() {
	u64 s1 = state0;
	u64 s0 = state1;
	state0 = s0;
	s1 ^= s1 << 23;
	s1 ^= s1 >> 17;
	s1 ^= s0;
	s1 ^= s0 >> 26;
	state1 = s1;
	return state0 + state1;
}

float randf()
{
	u32 fp = 0x3F800000 | (rand64() & 0x007FFFFF);
	float *a = (float *)&fp;
	return (*a) - 1.0f;
	
	// u32 r = rand64();
	// asm
	// (
	// 	"\tmov %1, %0\n"
	// 	"\tand $0x007FFFFF, %1\n"
	// 	"\tor $0x3F800000, %1\n"
	// 	: "=r" (r)
	// 	: "r" (r)
	// 	:
	// );
	// return *(float *)&r - 1.0f;
}
