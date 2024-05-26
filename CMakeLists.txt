cmake_minimum_required(VERSION 3.22)

project(HoMMybeer3)

include(cmake/FetchGitLib.cmake)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_COLOR_DIAGNOSTICS ON)

option(ENABLE_TESTS "Build tests" OFF)
option(ENABLE_COVERAGE "Run test coverage calculation" OFF)

if(ENABLE_COVERAGE AND NOT ENABLE_TESTS)
    message(FATAL_ERROR "Coverage can only be used together with a test build")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(WARNINGS
    -Werror
    -Wall
    -Wextra
    -Wpedantic
    -Wcast-align
    -Wcast-qual
    -Wdisabled-optimization
    -Wduplicated-branches
    -Wduplicated-cond
    -Wformat=2
    -Wlogical-op
    -Wmissing-include-dirs
    -Wpointer-arith
    -Wshadow=compatible-local
    -Wswitch-enum
    -Wvla
    -Wsuggest-attribute=const
    -Wdouble-promotion
    -Wconversion
    -Wundef
    -Wno-volatile
    -Wsuggest-override
    -Woverloaded-virtual
)

set(DEBUG_FLAGS
    -Og -g3 -ggdb
    CACHE INTERNAL ""
)
set(RELEASE_FLAGS
    -Os -g0
    CACHE INTERNAL ""
)

if(NOT ENABLE_TESTS)
    fetch_git_lib(
            SFML
            https://github.com/SFML/SFML.git
            2.6.0
    )
    add_executable(${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/app/main.cc)
    add_subdirectory(src/game_logic)
    add_subdirectory(src/game_engine)
    target_link_libraries(${PROJECT_NAME} PUBLIC game_logic game_engine sfml-graphics)
    target_compile_options(${PROJECT_NAME} PRIVATE ${WARNINGS} $<$<CONFIG:Debug>:${DEBUG_FLAGS}> $<$<CONFIG:Release>:${RELEASE_FLAGS}>)
else()
    fetch_git_lib(
            GoogleTest
            https://github.com/google/googletest/
            main
    )
    enable_testing()
    add_subdirectory(src/game_logic)
    add_subdirectory(tests)
    if(ENABLE_COVERAGE)
        # TODO: setup test coverage with CMake
    endif()
    
endif()