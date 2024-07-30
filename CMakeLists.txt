cmake_minimum_required(VERSION 3.13)
project(grpc_example
    VERSION 0.0.1
    DESCRIPTION "gRPC example for C++ and cmake"
    LANGUAGES CXX)

add_subdirectory(protos)
add_subdirectory(src)

