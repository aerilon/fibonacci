#include <iostream>

#include "api.h"

namespace fibonacci_api
{

void
dump(struct query &r)
{
	std::cout << unsigned(r.version) << std::endl;
	std::cout << unsigned(r.value) << std::endl;
	std::cout << std::hex << unsigned(r.checksum) << std::endl;
}

} /* fibonacci_api */
