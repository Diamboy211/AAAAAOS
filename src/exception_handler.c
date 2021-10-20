#include "exception_handler.h"

void exception_handler()
{
	*(long long *)0xA0000 = 0x0807060504030201;
}
