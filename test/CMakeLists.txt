cmake_minimum_required(VERSION 3.8)

find_package(GTest REQUIRED)

set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)

set(TEST_SRC_FILES
    TestFuzzySearch.cpp
)

add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
target_compile_options(${PROJECT_TEST_NAME} PRIVATE ${COMPILE_FLAGS})
target_compile_features(${PROJECT_TEST_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_TEST_NAME} PRIVATE fuzzy_search_lib GTest::GTest GTest::Main)

add_test(NAME all_tests COMMAND $<TARGET_FILE:${PROJECT_TEST_NAME}>)
