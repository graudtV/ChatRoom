#pragma once

#include <sfml/Network.hpp>

namespace chatroom {

enum class PacketType {
	eAuthentication,
	eCheckUsername,
	eServerResponse
};

/* Base class for all packets sent by client to server
 * and vice versa */
class Packet {
public:
	PacketType type() const { return m_type; }
	bool send_to(sf::TcpSocket& socket);
	sf::Packet pack();

	Packet(PacketType type) : m_type(type) {}

protected:
	virtual void pack_contents(sf::Packet& packet) = 0;

	/* prohibiting deletion by base class pointer */
	~Packet() {}
private:
	PacketType m_type;
};

/* Codes which can be sent by ServerResponsePacket */
enum class ServerResponseCode {
	eSuccess,
	eUserNotExist,
	eUserExists,
	eIncorrectPassword
};

/* Sent by: server
 * Contains: response for certain client requests where
 *   just a confirmation or some status is enough */
struct ServerResponsePacket : public Packet {
	ServerResponseCode code;

	explicit ServerResponsePacket(ServerResponseCode code_) :
		Packet(PacketType::eServerResponse),
		code(code_) {}

private:
	void pack_contents(sf::Packet& packet) override;
	static ServerResponsePacket unpack_contents(sf::Packet packet);
	template<class, class> friend class PacketVisitor;
};

/* Sent by: client
 * Contains: authentication request
 * Response: ServerResponsePacket with one of:
 *   eSuccess,
 *   eUserNotExist,
 *   eUserExists,
 *   eIncorrectPassword
 */
struct AuthenticationPacket : public Packet {
	std::string username;
	std::string password;
	bool create_account;

	AuthenticationPacket() : Packet(PacketType::eAuthentication) {}

	AuthenticationPacket(std::string_view username_, std::string_view password_, bool create_account_) :
		Packet(PacketType::eAuthentication),
		username(username_),
		password(password_),
		create_account(create_account_) {}

private:
	void pack_contents(sf::Packet& packet) override;
	static AuthenticationPacket unpack_contents(sf::Packet packet);
	template<class, class> friend class PacketVisitor;
};

/* Sent by: client
 * Contains: request to check if such user exists
 * Response: ServerResponsePacket with one of:
 *   eUserNotExist,
 *   eUserExists
 */
struct CheckUsernamePacket : public Packet {
	std::string username;

	CheckUsernamePacket() : Packet(PacketType::eCheckUsername) {}

	CheckUsernamePacket(std::string_view username_) :
		Packet(PacketType::eCheckUsername),
		username(username_) {}

private:
	void pack_contents(sf::Packet& packet) override;
	static CheckUsernamePacket unpack_contents(sf::Packet packet);
	template<class, class> friend class PacketVisitor;};

} // chatroom namespace end