cmake_minimum_required(VERSION 3.24)

project(HoMMybeer3)

include(cmake/FetchGitLib.cmake)

fetch_git_lib(
        GoogleTest
        https://github.com/google/googletest/
        main
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_TESTS "Build tests" OFF)
option(ENABLE_COVERAGE "Run test coverage calculation" OFF)

if(ENABLE_COVERAGE AND NOT ENABLE_TESTS)
    message(FATAL_ERROR "Coverage can only be used together with a test build")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS True)



if(NOT ENABLE_TESTS)
    add_executable(${PROJECT_NAME}
        ${CMAKE_CURRENT_SOURCE_DIR}/app/main.cc)
    
    target_link_libraries(${PROJECT_NAME} PUBLIC welcome)
else()
    enable_testing()
    add_subdirectory(tests)
    if(ENABLE_COVERAGE)
        # TODO: setup test coverage with CMake
    endif()
    
endif()

add_subdirectory(src)