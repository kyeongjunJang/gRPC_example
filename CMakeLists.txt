cmake_minimum_required(VERSION 3.10)

project(gRPC_example
        VERSION 0.0.1
        LANGUAGES CXX)

# Set CMake module path
set(CMAKE_MODULE_PATH "/usr/local/share/cmake-3.30" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "/usr/local/lib/cmake" ${CMAKE_MODULE_PATH})
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Debug message
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

# Dependencies
find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# Debug message
message(STATUS "Protobuf_FOUND: ${Protobuf_FOUND}")
message(STATUS "gRPC_FOUND: ${gRPC_FOUND}")

# Proto file
set(PROTO_FILE ${CMAKE_SOURCE_DIR}/protos/service.proto)
message(${PROTO_FILE})

set(PROTO_SRC_DIR ${CMAKE_CURRENT_BINARY_DIR}/proto-src)


# Generate the sources
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${PROTO_FILE})
GRPC_GENERATE_CPP(GRPC_SRCS GRPC_HDRS ${PROTO_FILE})

# Include directories
include_directories(${Protobuf_INCLUDE_DIRS} ${gRPC_INCLUDE_DIRS} ${CMAKE_BINARY_DIR})

# Source files
set(SOURCE_FILES
    src/main.cpp
    ${PROTO_SRCS}
    ${GRPC_SRCS}
)

# Executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link libraries
target_link_libraries(${PROJECT_NAME}
    ${Protobuf_LIBRARIES}
    ${GRPC_LIBRARIES}
    grpc++
)
