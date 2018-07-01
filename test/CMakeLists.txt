cmake_minimum_required(VERSION 3.2)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

add_definitions(${MSVC_COMPILER_DEFS})

set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)

set(TEST_SRC_FILES
    TestFuzzySearch.cpp
)

add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})

find_package(Threads)
target_link_libraries(${PROJECT_TEST_NAME} ${CMAKE_THREAD_LIBS_INIT} fuzzy_search_lib gtest gmock_main)
add_test(NAME all_tests COMMAND $<TARGET_FILE:${PROJECT_TEST_NAME}>)
