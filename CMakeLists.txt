cmake_minimum_required(VERSION 3.25)
project(czsocket VERSION 1.0.1 DESCRIPTION "socket library for chez scheme")
set(CMAKE_CXX_STANDARD 17)

add_library(${PROJECT_NAME} SHARED
        src/timer_handler.cpp
        src/timer_manager.cpp
        src/io_handler.cpp
        src/base_engine.cpp
        src/select_engine.cpp
)

add_executable(test
        src/main.cpp
)
target_link_libraries(test
        ${PROJECT_NAME}
)