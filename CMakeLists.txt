# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(MyNetworkingProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCE_FILES_UTILS src/utils/utils.cpp)
set(SOURCE_FILES_SERVER src/server/server.cpp)
set(SOURCE_FILES_CLIENT src/client/client.cpp)
set(SOURCE_FILES_EPOLL src/epoll/epoll.cpp)
set(SOURCE_FILES_SOCKET src/socket/socket.cpp)
set(SOURCE_FILES_INETADDR src/socket/inet_address.cpp)

# Executable for server
add_executable(server ${SOURCE_FILES_UTILS} ${SOURCE_FILES_SERVER} ${SOURCE_FILES_EPOLL} ${SOURCE_FILES_SOCKET} ${SOURCE_FILES_INETADDR})

# Executable for client
add_executable(client ${SOURCE_FILES_UTILS} ${SOURCE_FILES_CLIENT})

# Link libraries (if any, e.g., pthread)
# target_link_libraries(server ${EXTRA_LIBS})
# target_link_libraries(client ${EXTRA_LIBS})
