cmake_minimum_required(VERSION 3.12)

project(t3sting
    VERSION 1.0.1
    DESCRIPTION "Simple Testing Framework"
    LANGUAGES C
)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(TEST_VERBOSITY 1 
    CACHE
    STRING "Verbosity level for tests: 0=quiet,1=normal,2=verbose"
)

if(NOT TEST_VERBOSITY MATCHES "^[0-2]$")
    message(FATAL_ERROR "TEST_VERBOSITY must be 0, 1, or 2")
endif()

add_library(t3st STATIC
    src/assertions.c
    src/testing.c
    src/abort.c
)

target_include_directories(t3st PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(t3st PRIVATE TEST_VERBOSITY=${TEST_VERBOSITY})
