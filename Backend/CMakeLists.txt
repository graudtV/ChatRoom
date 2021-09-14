add_library(ClientBackend)

target_link_libraries(ClientBackend PRIVATE Support)

target_sources(ClientBackend PRIVATE
	client/Errors.cpp
	client/ResponseParser.cpp
	client/Server.cpp
	Packets.cpp
)