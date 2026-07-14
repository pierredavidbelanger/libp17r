cmake_minimum_required(VERSION 3.22)
project(p17r C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(IS_PROFILE_BUILD $<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>)

include(FetchContent)

set(TRACY_ENABLE ON CACHE BOOL "" FORCE)
FetchContent_Declare(tracy URL https://github.com/wolfpld/tracy/archive/refs/tags/v0.13.1.zip)
FetchContent_MakeAvailable(tracy)

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
set(ENABLE_CJSON_TEST OFF CACHE BOOL "" FORCE)
FetchContent_Declare(cjson URL https://github.com/davegamble/cjson/archive/refs/heads/master.zip)
FetchContent_MakeAvailable(cjson)

target_precompile_headers(
        raylib
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/p17r-mem.h
)

target_compile_definitions(
        raylib
        PRIVATE
        RL_MALLOC=p17r_malloc_internal
        RL_CALLOC=p17r_calloc_internal
        RL_REALLOC=p17r_realloc_internal
        RL_FREE=p17r_free_internal
        $<$<CONFIG:Release>:SUPPORT_TRACELOG=0>
)

add_library(
        ${PROJECT_NAME}
        STATIC
        include/p17r.h
        include/p17r-draw.h
        include/p17r-json.h
        include/p17r-log.h
        include/p17r-mem.h
        include/p17r-noise.h
        include/p17r-profile.h
        include/p17r-resources.h
        include/p17r-rl.h
        include/p17r-sched.h
        src/draw.c
        src/json.c
        src/log.c
        src/mem.c
        src/noise.c
        src/resources.c
        src/rl.c
        src/sched.c
)

target_compile_definitions(
        ${PROJECT_NAME}
        PUBLIC
        $<$<BOOL:${IS_PROFILE_BUILD}>:TRACY_ENABLE>
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
        PUBLIC
        $<$<BOOL:${IS_PROFILE_BUILD}>:TracyClient>
        PRIVATE
        raylib
        cjson
)
