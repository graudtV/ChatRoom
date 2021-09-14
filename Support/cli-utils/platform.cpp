#include "platform.h"
#include <iostream>

#if defined _WIN32 || defined _WIN64 || defined __CYGWIN__
# error "Windows is not supported. Fuck you."
#endif

#include <unistd.h>

namespace utils {
namespace platform {

void cls() {
	std::cerr << "\e[2J\e[H";
}

bool get_password(std::string& result) {
	if (char *str = getpass("")) {
		result = str;
		return true;
	}
	return false;
}

} // platform namespace end
} // utils namespace end