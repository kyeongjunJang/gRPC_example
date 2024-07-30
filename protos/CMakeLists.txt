# Find Protobuf and gRPC
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

set(PROTO_FILES
    weather.proto
    stock.proto
    soccer.proto
    )

add_library(protos ${PROTO_FILES})
target_link_libraries(protos
    PUBLIC
    gRPC::grpc
    gRPC::grpc++
    protobuf::libprotobuf
    )

target_include_directories(protos
    PUBLIC
    ${CMAKE_CURRENT_BINARY_DIR}
    )

get_target_property(protoc_location protobuf::protoc LOCATION)
get_target_property(grpc_cpp_plugin_location gRPC::grpc_cpp_plugin LOCATION)

protobuf_generate(
    TARGET protos
    LANGUAGE cpp
    GENERATE_EXTENSIONS .pb.h .pb.cc
    PLUGIN "protoc=${protoc_location}"
)
protobuf_generate(
    TARGET protos
    LANGUAGE grpc
    GENERATE_EXTENSIONS .grpc.pb.h .grpc.pb.cc
    PLUGIN "protoc-gen-grpc=${grpc_cpp_plugin_location}"
)