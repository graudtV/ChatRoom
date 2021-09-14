/* some platform dependent features */

#pragma once

#include <ostream>

namespace utils {
namespace platform {

void cls();
bool get_password(std::string& result);

} // platform namespace end
} // utils namespace end