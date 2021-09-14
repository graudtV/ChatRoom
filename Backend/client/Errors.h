#pragma once

#include "Support/ErrorBase.h"

namespace chatroom {
namespace client {

enum class ErrorCode {
	eNoResponse,
	eInvalidResponse,
	eUserNotExist,
	eUserExists,
	eIncorrectPassword
};

class Error : public ErrorBase<ErrorCode> {
public:
	static Error no_response();
	static Error invalid_response();
	static Error user_not_exist();
	static Error user_exists();
	static Error incorrect_password();

protected:
	Error(ErrorCode code, const char *descr) :
		ErrorBase<ErrorCode>(code, descr) {}
};

} // client namespace end
} // chatroom namespace end