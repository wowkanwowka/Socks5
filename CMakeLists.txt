cmake_minimum_required(VERSION 3.10)
project(Socks5)

find_package(Boost COMPONENTS system filesystem REQUIRED)

set(SRC Src/Socks5Client.cpp Src/Socks5Server.cpp)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Werror -Wall")

add_library(sockslib ${SRC})

#add_executable(Socks5 main.cpp)
add_executable(Socks5Client Client/ClientMain.cpp)
add_executable(Socks5Server Server/ServerMain.cpp)

set_target_properties(Socks5Client PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY Client
        LIBRARY_OUTPUT_DIRECTORY Client
        RUNTIME_OUTPUT_DIRECTORY Client)

set_target_properties(Socks5Server PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY Server
        LIBRARY_OUTPUT_DIRECTORY Server
        RUNTIME_OUTPUT_DIRECTORY Server)
#INSTALL(TARGETS Socks5Client DESTINATION Client)
#INSTALL(TARGETS Socks5Server DESTINATION Server)

#target_link_libraries(Socks5
#        sockslib
#        ${Boost_FILESYSTEM_LIBRARY}
#        ${Boost_SYSTEM_LIBRARY})

target_link_libraries(Socks5Client
        sockslib
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        glog)
target_link_libraries(Socks5Server
        sockslib
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
        glog)