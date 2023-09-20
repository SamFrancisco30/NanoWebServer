# Minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Project name
project(MyNetworkingProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Source files
set(SOURCE_FILES_UTILS src/utils/utils.cpp)
set(SOURCE_FILES_SERVER src/server/server.cpp)
set(SOURCE_FILES_CLIENT src/client/client.cpp)

# Executable for server
add_executable(server ${SOURCE_FILES_UTILS} ${SOURCE_FILES_SERVER})

# Executable for client
add_executable(client ${SOURCE_FILES_UTILS} ${SOURCE_FILES_CLIENT})

# Link libraries (if any, e.g., pthread)
# target_link_libraries(server ${EXTRA_LIBS})
# target_link_libraries(client ${EXTRA_LIBS})
