#include <FuzzySearch.h>
#include <benchmark/benchmark.h>

#include <cctype>
#include <gsl/gsl>

std::vector<std::string> NaiveSearch(const std::vector<std::string>& split_by_space, const std::vector<std::string>& files)
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
		else
		{
			continue;
		}
	}

	return results;
}

static const std::string& GetStringFunc(const std::string& string)
{
	return string;
}

#define FUZZY_SEARCH_BENCHMARK(BENCHMARK_NAME) BENCHMARK(BENCHMARK_NAME)->Repetitions(5)

void BM_FuzzyLongPattern(benchmark::State& state)
{
	std::vector<std::string> original_files = {
	    "e:/libs/nodehierarchy/main/source/BaseEntityNode.cpp",
	    "e:/libs/nodehierarchy/main/source/BaseEntityNode.h",
	    "e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.cpp",
	    "e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.h",
	    "e:/libs/nodehierarchy/main/source/BaseHierarchyNode.cpp",
	    "e:/libs/nodehierarchy/main/source/BaseHierarchyNode.h",
	    "e:/libs/nodehierarchy/main/source/BaseObjectNode.cpp",
	    "e:/libs/nodehierarchy/main/source/BaseObjectNode.h",
	    "e:/libs/nodehierarchy/main/source/CMakeLists.txt",
	    "e:/libs/otherlib/main/source/CMakeLists.txt",
	};

	for (const auto _ : state)
	{
		std::vector<FuzzySearch::SearchResult> results =
		    FuzzySearch::Search("hierarchy node base", original_files.begin(), original_files.end(), GetStringFunc, FuzzySearch::MatchMode::E_FILENAMES);
		benchmark::DoNotOptimize(results);
	}
}
FUZZY_SEARCH_BENCHMARK(BM_FuzzyLongPattern);

void BM_FindLongPattern(benchmark::State& state)
{
	std::vector<std::string> original_files = {
	    "e:/libs/nodehierarchy/main/source/BaseEntityNode.cpp",
	    "e:/libs/nodehierarchy/main/source/BaseEntityNode.h",
	    "e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.cpp",
	    "e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.h",
	    "e:/libs/nodehierarchy/main/source/BaseHierarchyNode.cpp",
	    "e:/libs/nodehierarchy/main/source/BaseHierarchyNode.h",
	    "e:/libs/nodehierarchy/main/source/BaseObjectNode.cpp",
	    "e:/libs/nodehierarchy/main/source/BaseObjectNode.h",
	    "e:/libs/nodehierarchy/main/source/CMakeLists.txt",
	    "e:/libs/otherlib/main/source/CMakeLists.txt",
	};

	std::vector<std::string> split_by_space = {"hierarchy", "node", "base"};

	for (const auto _ : state)
	{
		std::vector<std::string> results = NaiveSearch(split_by_space, original_files);
		benchmark::DoNotOptimize(results);
	}
}
FUZZY_SEARCH_BENCHMARK(BM_FindLongPattern);

void BM_FuzzyShortPattern(benchmark::State& state)
{
	std::vector<std::string> original_files = {
	    "e:/libs/nodehierarchy/main/source/BaseEntityNode.cpp",
	    "e:/libs/nodehierarchy/main/source/BaseEntityNode.h",
	    "e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.cpp",
	    "e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.h",
	    "e:/libs/nodehierarchy/main/source/BaseHierarchyNode.cpp",
	    "e:/libs/nodehierarchy/main/source/BaseHierarchyNode.h",
	    "e:/libs/nodehierarchy/main/source/BaseObjectNode.cpp",
	    "e:/libs/nodehierarchy/main/source/BaseObjectNode.h",
	    "e:/libs/nodehierarchy/main/source/CMakeLists.txt",
	    "e:/libs/otherlib/main/source/CMakeLists.txt",
	};

	for (const auto _ : state)
	{
		std::vector<FuzzySearch::SearchResult> results =
		    FuzzySearch::Search("BASE", original_files.begin(), original_files.end(), GetStringFunc, FuzzySearch::MatchMode::E_FILENAMES);
		benchmark::DoNotOptimize(results);
	}
}
FUZZY_SEARCH_BENCHMARK(BM_FuzzyShortPattern);

void BM_FindShortPattern(benchmark::State& state)
{
	std::vector<std::string> original_files = {
	    "e:/libs/nodehierarchy/main/source/BaseEntityNode.cpp",
	    "e:/libs/nodehierarchy/main/source/BaseEntityNode.h",
	    "e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.cpp",
	    "e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.h",
	    "e:/libs/nodehierarchy/main/source/BaseHierarchyNode.cpp",
	    "e:/libs/nodehierarchy/main/source/BaseHierarchyNode.h",
	    "e:/libs/nodehierarchy/main/source/BaseObjectNode.cpp",
	    "e:/libs/nodehierarchy/main/source/BaseObjectNode.h",
	    "e:/libs/nodehierarchy/main/source/CMakeLists.txt",
	    "e:/libs/otherlib/main/source/CMakeLists.txt",
	};

	std::vector<std::string> split_by_space = {"BASE"};

	for (const auto _ : state)
	{
		std::vector<std::string> results = NaiveSearch(split_by_space, original_files);
		benchmark::DoNotOptimize(results);
	}
}
FUZZY_SEARCH_BENCHMARK(BM_FindShortPattern);

BENCHMARK_MAIN();
