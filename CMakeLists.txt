cmake_minimum_required(VERSION 3.22)
project(p17r C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(arena
        URL https://github.com/tsoding/arena/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(arena)

FetchContent_Declare(raylib
        URL https://github.com/raysan5/raylib/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(rres
        URL https://github.com/raysan5/rres/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(rres)

FetchContent_Declare(osn
        URL https://github.com/smcameron/open-simplex-noise-in-c/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(osn)

add_library(${PROJECT_NAME}
        STATIC
        include/p17r.h
        include/p17r-mem.h
        include/p17r-noise.h
        include/p17r-rl.h
        include/p17r-sched.h
        src/mem.c
        src/noise.c
        src/rl.c
        src/sched.c)

target_compile_definitions(${PROJECT_NAME}
        PRIVATE
        $<$<CONFIG:Debug>:P17R_DEBUG=1>)

target_include_directories(
        ${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${FETCHCONTENT_BASE_DIR}/raylib-build/raylib/include
        ${FETCHCONTENT_BASE_DIR}/rres-src/src
        PRIVATE
        ${FETCHCONTENT_BASE_DIR}/arena-src
        ${FETCHCONTENT_BASE_DIR}/osn-src)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        raylib)
