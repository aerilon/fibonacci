#include "Backend.h"

uint64_t
Backend::compute(uint8_t n)
{
	if (n == 0)
		return 0;

	if (n == 1)
		return 1;

	return compute(n - 1) + compute(n - 2);
}
