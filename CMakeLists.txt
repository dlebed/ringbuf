cmake_minimum_required(VERSION 3.27)
project(ringbuf)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/f8d7d77c06936315286eb55f8de22cd23c188571.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        ringbuf_test
        ringbuf_test.cpp
)

target_include_directories(ringbuf_test PRIVATE include)

target_link_libraries(
        ringbuf_test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(ringbuf_test)

