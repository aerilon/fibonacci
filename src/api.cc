#include <iostream>

#include "api.h"

namespace fibonacci_api
{

void
dump(struct request &r)
{
	std::cout << unsigned(r._version) << std::endl;
	std::cout << unsigned(r._request) << std::endl;
	std::cout << std::hex << unsigned(r._checksum) << std::endl;
}

} /* fibonacci_api */

