#include "CLIMenu.h"
#include "platform.h"
#include <cassert>

namespace utils {

CLIMenu& CLIMenu::set_attempts(unsigned nattempts) {
	assert(nattempts > 0 && "nattempts must be > 0");
	m_nattempts = nattempts;
	return *this;
}

bool CLIMenu::process() {
	int option = eInvalidInput;
	auto nattempts = m_nattempts;
	assert(nattempts > 0 && "nattempts must be > 0");

	if (m_clear_screen)
		platform::cls();
	print_entries();
	do {
		print_prompt();
		option = parse_option();
		--nattempts;
	} while (nattempts > 0 && option == eInvalidInput);

	if (option == eInvalidInput) {
		*m_os << std::endl;
		if (m_attempt_limit_message.size())
			*m_os << m_attempt_limit_message << std::endl;
		return false;
	}
	if (option == eBrokenIstream) {
		*m_os << std::endl << std::endl;
		if (m_eof_message.size())
			*m_os << m_eof_message << std::endl;
		return false;
	}
	m_entries[option - 1].action(); // -1 because options are enumerated starting from 1
	return true;
}

void CLIMenu::print_entries() const {
	size_t i = 0;

	if (!m_title.empty()) {
		*m_os << m_title << std::endl;
		for (unsigned i = 0; i < m_title_indent; ++i)
			std::cout << std::endl;
	}
	for (auto&& entry : m_entries)
		*m_os << ++i << ") " << entry.text << std::endl;
}

void CLIMenu::print_prompt() const {
	*m_os << std::endl << "Enter option number: ";
}

int CLIMenu::parse_option() const {
	std::string input;
	char *end = nullptr;

	if (!std::getline(*m_is, input))
		return eBrokenIstream;
	auto res = std::strtol(input.c_str(), &end, 10);
	if (end != input.c_str() + input.size()) {
		*m_os << "Error: invalid input. Expected single integer number" << std::endl;
		return eInvalidInput;
	}
	if (res <= 0 || res > m_entries.size()) {
		*m_os << "Error: invalid option number. Expected number from 1 to " << m_entries.size() << std::endl;
		return eInvalidInput;
	}
	return res;
}

} // utils namespace end