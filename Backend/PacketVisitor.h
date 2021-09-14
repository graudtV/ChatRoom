#pragma once

#include "Packets.h"

namespace chatroom {

/* Helper class for handling different package types
 * Derive your class from it and "override" necessary on_xxx_packet() methods
 * Inspired by llvm::InstVisitor class */
template <class DerivedClass, class ReturnType>
class PacketVisitor {
protected:
	ReturnType unpack_packet(sf::Packet& packet);

	/* invoked if DerivedClass didn't specify any handler
	 * for this packet type. This method can also be
	 * "overrided" in DerivedClass */
	ReturnType on_unhandled_packet() { return ReturnType{}; }

	ReturnType on_authentication_packet(const AuthenticationPacket&) { return on_unhandled_packet(); }
	ReturnType on_server_response_packet(const ServerResponsePacket&) { return on_unhandled_packet(); }

	/* prohibiting deletion by base class pointer */
	~PacketVisitor() {}
};

template <class DerivedClass, class ReturnType>
ReturnType PacketVisitor<DerivedClass, ReturnType>::unpack_packet(sf::Packet& packet) {
	sf::Uint64 packet_type;
	packet >> packet_type;

	switch (static_cast<PacketType>(packet_type)) {
	case PacketType::eAuthentication:
		return static_cast<DerivedClass *>(this)->on_authentication_packet(AuthenticationPacket::unpack_contents(packet));
	case PacketType::eServerResponse:
		return static_cast<DerivedClass *>(this)->on_server_response_packet(ServerResponsePacket::unpack_contents(packet));
	default:
		throw std::runtime_error("Invalid packet type");
	}
}

} // chatroom namespace end