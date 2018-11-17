cmake_minimum_required(VERSION 3.8)

find_package(GTest REQUIRED)

set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_test)

set(TEST_SRC_FILES
    TestFuzzySearch.cpp
    TestHistoryModel.cpp
    #TestSearchableHistory.cpp
)

add_executable(${PROJECT_TEST_NAME} ${TEST_SRC_FILES})
target_compile_features(${PROJECT_TEST_NAME} PRIVATE cxx_std_17)
target_compile_options(${PROJECT_TEST_NAME} PRIVATE /wd4251 /wd4275)
target_link_libraries(${PROJECT_TEST_NAME} PUBLIC fuzzy_search_lib paste_history_window_lib Qt5::Test GTest::GTest GTest::Main)

add_test(NAME all_tests COMMAND $<TARGET_FILE:${PROJECT_TEST_NAME}>)
