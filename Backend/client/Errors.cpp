#include "Errors.h"

#define ERROR_DESCRIPTION(method, code, descr)     \
	Error Error::method() {                        \
		return { ErrorCode::code, descr };         \
	};

namespace chatroom {
namespace client {

ERROR_DESCRIPTION(no_response, eNoResponse, "server does not respond")
ERROR_DESCRIPTION(invalid_response, eInvalidResponse,
	"server sent something strange which cannot be properly handled "
	"by client. Probably it denotes a bug in server. "
	"Please report this bug (contacts can be found in help section on the main page)")
ERROR_DESCRIPTION(user_not_exist, eUserNotExist, "user not found")
ERROR_DESCRIPTION(user_exists, eUserExists, "such user already exists")
ERROR_DESCRIPTION(incorrect_password, eIncorrectPassword, "incorrect password")

} // client namespace end
} // chatroom namespace end