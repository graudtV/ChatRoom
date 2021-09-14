cmake_minimum_required(VERSION 3.13.0)

project(ChatRoom VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SFML COMPONENTS network REQUIRED)
find_package(clio REQUIRED)
link_libraries(sfml-network clio)

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(Support)
add_subdirectory(Backend)
add_subdirectory(Frontend/client-cli)