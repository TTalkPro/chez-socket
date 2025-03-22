cmake_minimum_required(VERSION 3.25)
project(czsocket VERSION 1.0.1 DESCRIPTION "socket library for chez scheme")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

set(SOURCE_FILES
        src/common/CBuffer.cpp
        src/common/CBaseEventLoop.cpp
        src/common/CBaseEventLoop.h
        src/common/CBaseHandler.cpp
        src/common/CBaseHandler.h
        src/common/CBaseTask.cpp
        src/common/CBaseTask.h
        src/common/CBaseReactor.cpp
        src/common/CBaseReactor.h
        src/common/CThreadedEventLoop.cpp
        src/common/CThreadedEventLoop.h
        src/common/CTimerManager.cpp
        src/common/CTimerManager.h
        src/common/CTimerTask.cpp
        src/common/CTimerTask.h
)

if(UNIX)
    set(SOURCE_FILES
            ${SOURCE_FILES}
            src/unix/core.cpp
            src/unix/pipe.cpp
            src/unix/UnixPlatform.h)
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

add_executable(test
        src/main.cpp
)
target_link_libraries(test
        ${PROJECT_NAME}
)