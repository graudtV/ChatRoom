#pragma once

#include "Backend/Packets.h"

namespace chatroom {
namespace client {

ServerResponseCode parse_response(sf::TcpSocket& socket);

} // client namespace end
} // chatroom namespace end