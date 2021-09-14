#pragma once

#include "Backend/common_defs.h"

namespace chatroom {
namespace client {

class Session;

class ChatRoom {
public:
	std::string name();
	std::string owner_username();

	bool has_owner_rights();
	void set_name(std::string_view name);
	void delete_chatroom();

	void send_message(const std::string& message);
	MessageId total_messages();
	std::vector<std::string> receive_messages(MessageId from, MessageId to);

	Session& session() { return m_session; }

private:
	ChatRoomId m_id;
	Session& m_session;

	ChatRoom(Session& session, ChatRoomId id) :
		m_session(session), m_id(id) {}
	friend class Session;
};

} // client namespace end
} // chatroom namespace end