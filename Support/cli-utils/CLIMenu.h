#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <functional>
#include <limits>
#include <iostream>

namespace utils {

class CLIMenu {
public:
	template <class Function, class... Args>
	CLIMenu& add_entry(std::string_view entry_text, Function&& function, Args&&... args);

	/* title is empty if not set. Empty title is not printed */
	CLIMenu& set_title(std::string_view title) { m_title = title; return *this; }
	
	/* indent = 0 if not set. If title is empty, indent is ignored */
	CLIMenu& set_indent_after_title(unsigned indent) { m_title_indent = indent; return *this; }
	
	/* os = std::cout if not set */
	CLIMenu& set_ostream(std::ostream& os) { m_os = &os; return *this; }
	
	/* is = std::cin if not set */
	CLIMenu& set_istream(std::istream& is) { m_is = &is; return *this; }

	/* nattempts = 3 if not set */
	CLIMenu& set_attempts(unsigned nattempts);

	/* enable = false if not set.
	 * If true, clears screen then parse() is called before printing menu */
	CLIMenu& clear_screen(bool enable = true) { m_clear_screen = enable; return *this; }

	/* no message if not set */
	CLIMenu& on_attempt_limit(std::string_view message) { m_attempt_limit_message = message; return *this; }

	/* no message if not set */
	CLIMenu& on_eof(std::string_view message) { m_eof_message = message; return *this; }

	/* print menu and parse user input */
	bool process();

private:
	struct Entry {
		std::string text;
		std::function<void ()> action;

		template <class Function>
		Entry(std::string_view entry_text, Function&& function) :
			text(entry_text), action(std::forward<Function>(function))
		{}

		template <class Function, class... Args>
		Entry(std::string_view entry_text, Function&& function, Args&&... args) :
			text(entry_text), action( [=]() { function(args...); } )
		{}
	};

	enum { eBrokenIstream = -1, eInvalidInput = -2 };

	std::vector<Entry> m_entries;
	std::string m_title;
	unsigned m_title_indent = 0;
	std::ostream *m_os = &std::cout;
	std::istream *m_is = &std::cin;
	unsigned m_nattempts = 3;
	bool m_clear_screen = false;
	std::string m_attempt_limit_message;
	std::string m_eof_message;

	void print_entries() const;
	void print_prompt() const;
	int parse_option() const;
};

template <class Function, class... Args>
CLIMenu& CLIMenu::add_entry(std::string_view entry_text, Function&& function, Args&&... args) {
	m_entries.emplace_back(entry_text, std::forward<Function>(function), std::forward<Args>(args)...);
	return *this;
}

} // utils namespace end