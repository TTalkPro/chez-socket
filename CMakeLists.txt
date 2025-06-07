cmake_minimum_required(VERSION 3.25)
project(lisp_async VERSION 1.0.1 DESCRIPTION "A simpe async library for LISP")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)

set(SOURCE_FILES
        src/LispAsync.h
        src/common/CBuffer.cpp
        src/common/CEventLoop.cpp
        src/common/CEventLoop.h
        src/common/CBaseEvent.cpp
        src/common/CBaseEvent.h
        src/common/CBaseTask.cpp
        src/common/CBaseTask.h
        src/common/CBaseReactor.cpp
        src/common/CBaseReactor.h
        src/common/CTaskManager.cpp
        src/common/CTaskManager.h
        src/common/LispAsyncWrapper.h
        src/common/LispAsyncWrapper.cpp
)

if(UNIX)
    set(SOURCE_FILES
            ${SOURCE_FILES}
            src/unix/core.cpp
            src/unix/pipe.cpp
            src/unix/UnixPlatform.h
            src/unix/CWakeupEvent.h
            src/unix/CWakeupEvent.cpp
            src/unix/CSelectReactor.h
            src/unix/CSelectReactor.cpp)
endif()

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

add_executable(test
        src/main.cpp
)
target_link_libraries(test
        ${PROJECT_NAME}
)