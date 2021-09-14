#pragma once

#include <stdexcept>

namespace chatroom {

template <class ErrorCode>
class ErrorBase : public std::runtime_error {
public:
	using ErrorCodeType = ErrorCode;

	ErrorCode code() const { return m_code; }

protected:
	ErrorBase(ErrorCode code, const char *descr) :
		std::runtime_error(descr),
		m_code(code)
	{}

private:
	ErrorCode m_code;
};

} // chatroom namespace end