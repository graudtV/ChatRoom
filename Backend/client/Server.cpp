#include "Server.h"
#include "Errors.h"
#include "ResponseParser.h"

namespace chatroom {
namespace client {

Server::Server(sf::IpAddress ip, unsigned port) {
	if (m_socket.connect(ip, port, sf::seconds(5)) != sf::Socket::Done)
		throw Error::no_response();
}

Session Server::sign_in(std::string_view username, std::string_view password) {
	AuthenticationPacket packet(username, password, false);
	if (!packet.send_to(m_socket))
		throw Error::no_response();
	switch (parse_response(m_socket)) {
	case ServerResponseCode::eSuccess: return Session(*this);
	case ServerResponseCode::eUserNotExist: throw Error::user_not_exist();
	case ServerResponseCode::eIncorrectPassword: throw Error::incorrect_password();
	default: throw Error::invalid_response();
	}
}

Session Server::sign_up(std::string_view username, std::string_view password) {
	AuthenticationPacket packet(username, password, false);
	if (!packet.send_to(m_socket))
		throw Error::no_response();
	switch (parse_response(m_socket)) {
	case ServerResponseCode::eSuccess: return Session(*this);
	case ServerResponseCode::eUserExists: throw Error::user_exists();
	case ServerResponseCode::eIncorrectPassword: throw Error::incorrect_password();
	default: throw Error::invalid_response();
	}
}

bool Server::user_exists(std::string_view username) {
	CheckUsernamePacket packet(username);
	if (!packet.send_to(m_socket))
		throw Error::no_response();
	switch (parse_response(m_socket)) {
	case ServerResponseCode::eUserExists: return true;
	case ServerResponseCode::eUserNotExist: return false;
	default: throw Error::invalid_response();
	}
}

} // client namespace end
} // chatroom namespace end