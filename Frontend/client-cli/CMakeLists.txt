add_executable(chatroom-cli)

target_link_libraries(chatroom-cli PRIVATE ClientBackend)

target_sources(chatroom-cli PRIVATE
	CLI.cpp
	cli_main.cpp
)