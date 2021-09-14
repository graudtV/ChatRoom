#pragma once

#include "ChatRoom.h"

namespace chatroom {
namespace client {

class Server;

/* Represents user session, i.e. when a user is logged in
 * onto some server and can query for available ChatRooms */
class Session {
public:
	std::string user_login();
	std::vector<ChatRoom> chatrooms();
	ChatRoom create_chatroom();

	Server& server() { return m_server; }

private:
	Server& m_server;

	Session(Server& server) : m_server(server) {}
	friend class Server;
};

} // client namespace end
} // chatroom namespace en