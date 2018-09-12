cmake_minimum_required(VERSION 3.8)

find_package(benchmark CONFIG REQUIRED)

set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_benchmark)

add_executable(fuzzy_search_benchmark FuzzySearchBenchmark.cpp)
target_compile_features(fuzzy_search_benchmark PUBLIC cxx_std_14)
target_link_libraries(fuzzy_search_benchmark PRIVATE fuzzy_search_lib benchmark::benchmark benchmark::benchmark_main)
