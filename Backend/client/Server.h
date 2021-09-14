#pragma once

#include "Session.h"
#include <memory>

namespace chatroom {
namespace client {

/* client interface for connecting ChatRoom server and entering sessions */
class Server {
public:
	Server(sf::IpAddress ip, unsigned port);

	Session sign_in(std::string_view username, std::string_view password);
	Session sign_up(std::string_view username, std::string_view password);
	bool user_exists(std::string_view username);

private:
	sf::TcpSocket m_socket;

	sf::TcpSocket& socket() { return m_socket; }
	friend class Session;
};

} // client namespace end
} // chatroom namespace end