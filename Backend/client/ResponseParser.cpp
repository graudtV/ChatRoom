#include "ResponseParser.h"
#include "Backend/PacketVisitor.h"
#include "Errors.h"

namespace chatroom {
namespace client {
namespace {

class ResponseParser : public PacketVisitor<ResponseParser, ServerResponseCode> {
public:
	ServerResponseCode get_code(sf::TcpSocket& socket) {
		sf::Packet result;
		if (socket.receive(result) != sf::Socket::Done)
			throw Error::no_response();
		return unpack_packet(result);
	}

private:
	ServerResponseCode on_unhandled_packet() {
		throw Error::invalid_response();
	}

	ServerResponseCode on_server_response_packet(ServerResponsePacket packet) {
		return packet.code;
	}

	friend class PacketVisitor<ResponseParser, ServerResponseCode>;
};

} // anonymous namespace end

ServerResponseCode parse_response(sf::TcpSocket& socket) {
	static ResponseParser parser;
	return parser.get_code(socket);
}

} // client namespace end
} // chatroom namespace end