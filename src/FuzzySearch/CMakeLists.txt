cmake_minimum_required(VERSION 3.8)

project (fuzzy_search_lib)

set(FUZZY_SEARCH_SOURCE_FILES
        FuzzySearch.cpp
        FuzzySearch.h
        )

add_library(fuzzy_search_lib ${FUZZY_SEARCH_SOURCE_FILES})
target_compile_options(fuzzy_search_lib PRIVATE ${COMPILE_FLAGS})
target_compile_features(fuzzy_search_lib PUBLIC cxx_std_17)
target_include_directories(fuzzy_search_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${GSL_INCLUDE_DIR})

enable_guideline_checker(fuzzy_search_lib)

