#include "CLI.h"
#include "Support/cli-utils/CLIMenu.h"
#include "Support/cli-utils/platform.h"
#include "Backend/client/Errors.h"
#include <clio/Input.h>
#include <cassert>

namespace chatroom {

namespace {

const char *eof_message = 
	"ChatRoom: EOF received, exiting\n"
	"Thanks for trying ChatRoom!\n";

utils::CLIMenu create_menu() {
	return utils::CLIMenu()
		.set_indent_after_title(1)
		.on_attempt_limit(
			"Ooopppsss, too many incorrect attempts...\n"
			"Please restart ChatRoom if you want to try again\n")
		.on_eof(eof_message);
}

void wait_confirmation() {
	std::cerr << std::endl << "Please enter any symbol to try again... ";
	std::string unused;
	std::getline(std::cin, unused);
}

} // anonymous namespace dn


void CLI::greeting_page() {
	create_menu()
		.clear_screen()
		.set_title(
			"Welcome to command line version of ChatRoom!\n"
			"Please select one of the following:")
		.add_entry("connect ChatRoom server", std::bind(&CLI::connect_server, this))
		.add_entry("help & about", std::bind(&CLI::print_help, this))
		.process();
}

void CLI::connect_server() {
	while (1) {
		utils::platform::cls();
		std::cout << "Please enter server IP and port" << std::endl << std::endl;

		switch (try_connect_server()) {
			case AttemptResult::eSuccess: authentication_page(); return;
			case AttemptResult::eFatalError: return;
			case AttemptResult::eAgain: continue;
		}
	}
}

void CLI::print_help() {
	utils::platform::cls();
	std::cout << "TBD" << std::endl;
}

void CLI::authentication_page() {
	create_menu()
		.clear_screen()
		.set_title("Please select authentication method")
		.add_entry("sign in", std::bind(&CLI::authenticate, this, false))
		.add_entry("sing up", std::bind(&CLI::authenticate, this, true))
		.process();
}

void CLI::authenticate(bool create_account) {
	while (1) {
		utils::platform::cls();
		std::cout << ((create_account) 
			? "Please enter your username and password"
			: "Please select username and password")
			<< std::endl << std::endl;
		switch (try_authenticate(create_account)) {
			case AttemptResult::eSuccess: chatroom_list_page(); return;
			case AttemptResult::eFatalError: return;
			case AttemptResult::eAgain: continue;
		}
	}
}

void CLI::chatroom_list_page() {
	std::cout << "There will be a list of chatrooms" << std::endl;
	throw std::runtime_error("not implemented");
}

CLI::AttemptResult CLI::try_connect_server() try {
	clio::CmdlineInput input;
	std::string ipstr = input.get_value<std::string>("Server IP", clio::single_word);
	sf::IpAddress ip(ipstr);
	if (ip == sf::IpAddress::None) {
		std::cerr << "Error: '" << ipstr << "' is not a valid IP address" << std::endl;
		wait_confirmation();
		return AttemptResult::eAgain;
	}
	auto port = input.get_value<uint16_t>("Server port");
	std::cout << std::endl << "Connecting server ...";
	m_server = std::make_unique<client::Server>(ip, port);	
	return AttemptResult::eSuccess;
} catch (clio::CmdlineInput::IOError& e) {
	std::cerr << std::endl << std::endl << eof_message << std::endl;
	return AttemptResult::eFatalError;
} catch (std::runtime_error& e) {
	std::cerr << std::endl << "Error: " << e.what() << std::endl;
	wait_confirmation();
	return AttemptResult::eAgain;
}

CLI::AttemptResult CLI::try_authenticate(bool create_account) try {
	clio::CmdlineInput input;
	auto username = input.get_value<std::string>("username", clio::single_word);

	bool user_exists = m_server->user_exists(username);
	if (create_account && user_exists) {
		std::cerr << std::endl << "Error: user '" << username << "' already exists" << std::endl;
		wait_confirmation();
		return AttemptResult::eAgain;
	} else if (!create_account && !user_exists) {
		std::cerr
			<< std::endl
			<< "Error: user '" << username << "' doesn't exist" << std::endl
			<< "Please check if login is correct or sign up if you are not registered yet" << std::endl;
		wait_confirmation();
		return AttemptResult::eAgain;
	}

	auto password = input.get_value<std::string>("password", clio::single_word);

	if (!create_account) {
		m_session = std::make_unique<client::Session>(
			m_server->sign_in(username, password));		
	} else {
		m_session = std::make_unique<client::Session>(
			m_server->sign_up(username, password));
	}
	return AttemptResult::eSuccess;
} catch (clio::CmdlineInput::IOError& e) {
	std::cerr << std::endl << std::endl << eof_message << std::endl;
	return AttemptResult::eFatalError;
} catch (std::runtime_error& e) {
	std::cerr << std::endl << "Error: " << e.what() << std::endl;
	wait_confirmation();
	return AttemptResult::eAgain;
}

} // chatroom namespace end