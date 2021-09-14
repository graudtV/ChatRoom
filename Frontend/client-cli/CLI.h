#pragma once

#include "Backend/client/Server.h"
#include <memory>

namespace chatroom {

class CLI {
public:
	void run() { greeting_page(); }

private:
	std::unique_ptr<client::Server> m_server;
	std::unique_ptr<client::Session> m_session;
	std::unique_ptr<client::ChatRoom> m_chatroom;

	enum class AttemptResult { eSuccess, eAgain, eFatalError };

	void greeting_page();
		void connect_server();
		void print_help();

	void authentication_page();
		void authenticate(bool create_account);

	void chatroom_list_page();

	AttemptResult try_connect_server();
	AttemptResult try_authenticate(bool create_account);
};

} // chatroom namespace end