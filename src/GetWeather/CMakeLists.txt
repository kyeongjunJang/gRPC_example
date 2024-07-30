project(GetWeather
    VERSION 0.0.1
    LANGUAGES CXX)


foreach(_target
    GetWeather_client GetWeather_server
    )
    add_executable(${_target} ${_target}.cpp)
    target_link_libraries(${_target}
        PRIVATE
            protos
            # gRPC::grpc++
            # protobuf::libprotobuf
        )
    # target_include_directories(${_target}
    #     PRIVATE
    #     ${CMAKE_CURRENT_BINARY_DIR}
    #     )
endforeach()
