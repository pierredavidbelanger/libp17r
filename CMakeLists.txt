cmake_minimum_required(VERSION 3.22)
project(p17r C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(arena URL https://github.com/tsoding/arena/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(arena)

FetchContent_Declare(raylib URL https://github.com/raysan5/raylib/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(rres URL https://github.com/raysan5/rres/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(rres)

FetchContent_Declare(raygui URL https://github.com/raysan5/raygui/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(raygui)

FetchContent_Declare(osn URL https://github.com/smcameron/open-simplex-noise-in-c/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(osn)

FetchContent_Declare(fnl URL https://github.com/Auburn/FastNoiseLite/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(fnl)

set(ENABLE_CUSTOM_COMPILER_FLAGS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(cjson URL https://github.com/davegamble/cjson/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(cjson)

target_compile_definitions(
        raylib
        PRIVATE
        $<$<CONFIG:Release>:SUPPORT_TRACELOG=0>
)

add_library(
        ${PROJECT_NAME}
        STATIC
        include/p17r.h
        include/p17r-json.h
        include/p17r-log.h
        include/p17r-mem.h
        include/p17r-noise.h
        include/p17r-rl.h
        include/p17r-sched.h
        src/json.c
        src/log.c
        src/mem.c
        src/noise.c
        src/rl.c
        src/sched.c
)

target_compile_options(
        ${PROJECT_NAME}
        PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Werror
)

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_include_directories(
        ${PROJECT_NAME}
        SYSTEM PUBLIC
        ${FETCHCONTENT_BASE_DIR}/raylib-build/raylib/include
        ${FETCHCONTENT_BASE_DIR}/rres-src/src
        ${FETCHCONTENT_BASE_DIR}/raygui-src/src
        ${FETCHCONTENT_BASE_DIR}/fnl-src/C
        ${FETCHCONTENT_BASE_DIR}/cjson-src
)

target_include_directories(
        ${PROJECT_NAME}
        SYSTEM PRIVATE
        ${FETCHCONTENT_BASE_DIR}/arena-src
        ${FETCHCONTENT_BASE_DIR}/osn-src
)

target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE
        raylib
        cjson
)
