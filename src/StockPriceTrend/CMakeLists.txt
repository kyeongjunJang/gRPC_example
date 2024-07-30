project(StockPriceTrend
    VERSION 0.0.1
    LANGUAGES CXX)

foreach(_target
StockPriceTrend_client StockPriceTrend_server
    )
    add_executable(${_target} ${_target}.cpp)
    target_link_libraries(${_target}
        PRIVATE
            protos
        )
endforeach()
