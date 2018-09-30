cmake_minimum_required(VERSION 3.8)

set(SRC_FILES
        SearchableHistory.h
        SearchableHistory.cpp
        )


add_library(searchable_history_lib ${SRC_FILES})
target_compile_options(searchable_history_lib PRIVATE ${COMPILE_FLAGS})
target_compile_features(searchable_history_lib PUBLIC cxx_std_17)
if(MSVC)
  target_compile_options(searchable_history_lib PRIVATE /wd4018 /wd4146 /wd4244 /wd4251 /wd4267 /wd4305 /wd4355 /wd4800 /wd4996)
endif()
target_link_libraries(searchable_history_lib PUBLIC history_types_lib fuzzy_search_lib)
target_include_directories(searchable_history_lib PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

enable_guideline_checker(searchable_history_lib)