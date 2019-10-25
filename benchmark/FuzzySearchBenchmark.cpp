#include <algorithm>
#include <cctype>
#include <gsl/gsl>
#include <regex>

#include <benchmark/benchmark.h>

#include "Files.h"
#include <FuzzySearch.h>

std::vector<std::string> StringSearch(const std::vector<std::string>& split_by_space, const std::vector<std::string>& files)
{
	std::vector<std::string> results;
	results.reserve(files.size());

	for (std::string file : files)
	{
		std::transform(file.begin(), file.end(), file.begin(), [](char c) { return gsl::narrow_cast<char>(std::tolower(c)); });

		size_t found_count = 0;
		for (const std::string& str : split_by_space)
		{
			const auto found = file.find_last_of(str);
			if (found != std::string::npos)
			{
				++found_count;
			}
			else
			{
				break;
			}
		}

		if (found_count == split_by_space.size())
		{
			results.push_back(file);
		}
	}

	return results;
}

std::vector<std::string> RegexSearch(const std::vector<std::string>& split_by_space, const std::vector<std::string>& files)
{
	std::vector<std::string> results;
	results.reserve(files.size());
	for (const std::string& str : split_by_space)
	{
		std::regex self_regex(str, std::regex_constants::icase);
		for (const std::string& file : files)
		{
			if (std::regex_search(file, self_regex))
			{
				results.push_back(file);
			}
		}
	}
	return results;
}

std::vector<std::string> BoyerMoore(const std::vector<std::string>& split_by_space, const std::vector<std::string>& files)
{
	std::vector<std::string> results;
	results.reserve(files.size());
	for (const std::string& str : split_by_space)
	{
		for (const std::string& file : files)
		{
			auto it = std::search(file.cbegin(), file.cend(), std::boyer_moore_horspool_searcher(str.begin(), str.end()));
			if (it != file.end())
			{
				results.push_back(file);
			}
		}
	}
	return results;
}

static std::string_view GetStringFunc(std::string_view string)
{
	return string;
}

#define FUZZY_SEARCH_BENCHMARK(BENCHMARK_NAME) BENCHMARK(BENCHMARK_NAME)->Repetitions(2)->Unit(benchmark::kMicrosecond)

void BM_FuzzyLongPattern(benchmark::State& state)
{
	std::vector<std::string> files;
	for (const char* str : FuzzySearchBenchmark::FILES)
	{
		files.push_back(str);
	}

	for (const auto _ : state)
	{
		std::vector<FuzzySearch::SearchResult> results =
		    FuzzySearch::Search("qt base view list", files.begin(), files.end(), &GetStringFunc, FuzzySearch::MatchMode::E_SOURCE_FILES);
		benchmark::DoNotOptimize(results);
	}
}
FUZZY_SEARCH_BENCHMARK(BM_FuzzyLongPattern);

void BM_FuzzyShortPattern(benchmark::State& state)
{
	std::vector<std::string> files;
	for (const char* str : FuzzySearchBenchmark::FILES)
	{
		files.push_back(str);
	}

	for (const auto _ : state)
	{
		std::vector<FuzzySearch::SearchResult> results =
		    FuzzySearch::Search("TABLE", files.begin(), files.end(), &GetStringFunc, FuzzySearch::MatchMode::E_SOURCE_FILES);
		benchmark::DoNotOptimize(results);
	}
}
FUZZY_SEARCH_BENCHMARK(BM_FuzzyShortPattern);

void BM_BoyerMooreLongPattern(benchmark::State& state)
{
	std::vector<std::string> files;
	for (const char* str : FuzzySearchBenchmark::FILES)
	{
		files.push_back(str);
	}

	std::vector<std::string> split_by_space = {"qt", "base", "view", "list"};

	for (const auto _ : state)
	{
		std::vector<std::string> results = BoyerMoore(split_by_space, files);
		benchmark::DoNotOptimize(results);
	}
}
FUZZY_SEARCH_BENCHMARK(BM_BoyerMooreLongPattern);

void BM_BoyerMooreShortPattern(benchmark::State& state)
{
	std::vector<std::string> files;
	for (const char* str : FuzzySearchBenchmark::FILES)
	{
		files.push_back(str);
	}

	std::vector<std::string> split_by_space = {"TABLE"};

	for (const auto _ : state)
	{
		std::vector<std::string> results = BoyerMoore(split_by_space, files);
		benchmark::DoNotOptimize(results);
	}
}
FUZZY_SEARCH_BENCHMARK(BM_BoyerMooreShortPattern);

void BM_FindLongPattern(benchmark::State& state)
{
	std::vector<std::string> files;
	for (const char* str : FuzzySearchBenchmark::FILES)
	{
		files.push_back(str);
	}

	std::vector<std::string> split_by_space = {"qt", "base", "view", "list"};

	for (const auto _ : state)
	{
		std::vector<std::string> results = StringSearch(split_by_space, files);
		benchmark::DoNotOptimize(results);
	}
}
FUZZY_SEARCH_BENCHMARK(BM_FindLongPattern);

void BM_FindShortPattern(benchmark::State& state)
{
	std::vector<std::string> files;
	for (const char* str : FuzzySearchBenchmark::FILES)
	{
		files.push_back(str);
	}

	std::vector<std::string> split_by_space = {"TABLE"};

	for (const auto _ : state)
	{
		std::vector<std::string> results = StringSearch(split_by_space, files);
		benchmark::DoNotOptimize(results);
	}
}
FUZZY_SEARCH_BENCHMARK(BM_FindShortPattern);

void BM_RegexLongPattern(benchmark::State& state)
{
	std::vector<std::string> files;
	for (const char* str : FuzzySearchBenchmark::FILES)
	{
		files.push_back(str);
	}

	std::vector<std::string> split_by_space = {"qt", "base", "view", "list"};

	for (const auto _ : state)
	{
		std::vector<std::string> results = RegexSearch(split_by_space, files);
		benchmark::DoNotOptimize(results);
	}
}
FUZZY_SEARCH_BENCHMARK(BM_RegexLongPattern);

void BM_RegexShortPattern(benchmark::State& state)
{
	std::vector<std::string> files;
	for (const char* str : FuzzySearchBenchmark::FILES)
	{
		files.push_back(str);
	}

	std::vector<std::string> split_by_space = {"TABLE"};

	for (const auto _ : state)
	{
		std::vector<std::string> results = RegexSearch(split_by_space, files);
		benchmark::DoNotOptimize(results);
	}
}
FUZZY_SEARCH_BENCHMARK(BM_RegexShortPattern);

BENCHMARK_MAIN();
