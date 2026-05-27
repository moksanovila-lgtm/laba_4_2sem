cmake_minimum_required(VERSION 3.14)
project(Lab4Tests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(LAB4_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../src)
set(LAB2_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/Lab_2/library/include)

set(TEST_SOURCES
    test_lazy_sequence.cpp
    test_readonly_stream.cpp
    test_online_statistics.cpp
)

add_executable(lab4_tests ${TEST_SOURCES})

target_link_libraries(lab4_tests
    GTest::gtest_main
)

target_include_directories(lab4_tests PRIVATE
    ${LAB4_SRC}
    ${LAB2_INCLUDE}
)

include(GoogleTest)
gtest_discover_tests(lab4_tests)