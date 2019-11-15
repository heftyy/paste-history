cmake_minimum_required(VERSION 3.8)

find_package(benchmark CONFIG REQUIRED)

set(PROJECT_TEST_NAME ${PROJECT_NAME_STR}_benchmark)

set_target_properties(benchmark::benchmark benchmark::benchmark_main 
					  PROPERTIES MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release MAP_IMPORTED_CONFIG_MINSIZEREL Release)

add_executable(fuzzy_search_benchmark FuzzySearchBenchmark.cpp Files.h)
target_compile_features(fuzzy_search_benchmark PUBLIC cxx_std_17)
target_link_libraries(fuzzy_search_benchmark PRIVATE fuzzy_search_lib benchmark::benchmark benchmark::benchmark_main)

add_executable(history_model_benchmark HistoryModelBenchmark.cpp Files.h)
target_compile_features(history_model_benchmark PUBLIC cxx_std_17)
target_link_libraries(history_model_benchmark PRIVATE paste_history_window_lib fuzzy_search_lib benchmark::benchmark benchmark::benchmark_main)
