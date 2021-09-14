#include "Packets.h"

namespace chatroom {

bool Packet::send_to(sf::TcpSocket& socket) {
	auto packet = pack();
	return socket.send(packet) == sf::Socket::Done;
}

sf::Packet Packet::pack() {
	sf::Packet result;
	result << static_cast<sf::Uint64>(m_type);
	pack_contents(result);
	return result;
}

void ServerResponsePacket::pack_contents(sf::Packet& packet) {
	packet << static_cast<sf::Uint64>(code);
}

ServerResponsePacket ServerResponsePacket::unpack_contents(sf::Packet packet) {
	sf::Uint64 code;
	packet >> code;
	return ServerResponsePacket(static_cast<ServerResponseCode>(code));
}

void AuthenticationPacket::pack_contents(sf::Packet& packet) {
	packet << username << password << create_account;
}

AuthenticationPacket AuthenticationPacket::unpack_contents(sf::Packet packet) {
	AuthenticationPacket result;
	packet >> result.username >> result.password >> result.create_account;
	return result;
}

void CheckUsernamePacket::pack_contents(sf::Packet& packet) {
	packet << username;
}

CheckUsernamePacket CheckUsernamePacket::unpack_contents(sf::Packet packet) {
	CheckUsernamePacket result;
	packet >> result.username;
	return result;
}

} // chatroom namespace end