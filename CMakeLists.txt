cmake_minimum_required(VERSION 3.15)
project(tape)

set(CMAKE_CXX_STANDARD 17)

option(BUILD_TESTS OFF)

set(SOURCES src/file_tape.cpp src/tape_sorting.cpp)
set(HEADERS include/tape_interface.h include/file_tape.h include/tape_sorting.h)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL
  https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG        v2.2.1
)
FetchContent_MakeAvailable(cxxopts)

add_library(tape-lib STATIC ${SOURCES} ${HEADERS})
target_include_directories(tape-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_precompile_headers(tape-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h)

add_executable(tape src/main.cpp)
target_link_libraries(tape PUBLIC tape-lib cxxopts)


if(BUILD_TESTS)
    enable_testing()
    add_executable(test-tape test/main.cpp test/file_tape_test.h test/tape_config_test.h test/tape_sorting_test.h test/test_utils.h)
    target_link_libraries(test-tape PUBLIC tape-lib gtest_main)
    include(GoogleTest)
    gtest_discover_tests(test-tape)

    include(CTest)
    add_test(NAME all_tests COMMAND test-tape CONFIGURATION Debug)

endif() # BUILD_TESTS