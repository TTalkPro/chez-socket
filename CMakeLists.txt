cmake_minimum_required(VERSION 3.25)
project(czsocket VERSION 1.0.1 DESCRIPTION "socket library for chez scheme")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

set(SOURCE_FILES
        src/common/cbuffer.cpp
        src/common/base_handler.cpp
        src/common/timer_handler.cpp
        src/common/io_handler.cpp
        src/common/wakeup_operation.cpp
        src/common/wakeup_handler.cpp
        src/common/timer_manager.cpp
        src/common/io_manager.cpp
        src/reactor.cpp)

if(UNIX)
    set(SOURCE_FILES
            ${SOURCE_FILES}
            src/unix/core.cpp
            src/unix/pipe.cpp
            src/unix/socket.cpp
            src/unix/select_io_manager.cpp
            src/unix/tcp_handler.cpp
            src/unix/unix_wakeup_handler.cpp)
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

add_executable(test
        src/main.cpp
)
target_link_libraries(test
        ${PROJECT_NAME}
)