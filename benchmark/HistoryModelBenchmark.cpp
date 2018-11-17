#include <cctype>
#include <gsl/gsl>
#include <iostream>

#include <benchmark/benchmark.h>

#include <HistoryItem.h>
#include <HistoryItemModel.h>
#include <QStandardItemModel>

#include <FuzzySearch.h>

static size_t GetTimestamp()
{
	auto duration = std::chrono::system_clock::now().time_since_epoch();
	return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

struct StringWithScore
{
	std::string m_String;
	int m_MatchScore;
};

static std::string_view GetStringFunc(const StringWithScore& obj)
{
	return obj.m_String;
}

#define HISTORY_MODEL_BENCHMARK(BENCHMARK_NAME) BENCHMARK(BENCHMARK_NAME)->Range(8, 8 << 10);

void BM_ModelInsert(benchmark::State& state)
{
	std::vector<std::string> strings = {
	    "git init",
	    "git status",
	    "git add my_new_file.txt",
	    "git commit -m \"Add three files\"",
	    "git reset --soft HEAD^",
	    "git remote add origin https://github.com/heftyy/fuzzy-search.git",
	};

	HistoryItemModel history_model(nullptr);

	for (const auto _ : state)
	{
		for (auto i = 0; i < state.range(0); ++i)
		{
			for (const std::string& text : strings)
			{
				std::hash<std::string> hasher;
				const size_t text_hash = hasher(text);
				history_model.AddToHistory({text, text_hash, GetTimestamp()});
			}
		}
		history_model.Clear();
	}
}
HISTORY_MODEL_BENCHMARK(BM_ModelInsert);

void BM_BasicInsert(benchmark::State& state)
{
	std::vector<std::string> strings = {
	    "git init",
	    "git status",
	    "git add my_new_file.txt",
	    "git commit -m \"Add three files\"",
	    "git reset --soft HEAD^",
	    "git remote add origin https://github.com/heftyy/fuzzy-search.git",
	};

	std::vector<HistoryItem> history_items;

	for (const auto _ : state)
	{
		for (auto i = 0; i < state.range(0); ++i)
		{
			for (const std::string& text : strings)
			{
				std::hash<std::string> hasher;
				const size_t text_hash = hasher(text);
				history_items.emplace_back(HistoryItemData{text, text_hash, GetTimestamp()});
			}
		}
	}
}
HISTORY_MODEL_BENCHMARK(BM_BasicInsert);

void BM_ModelFilterAndSort(benchmark::State& state)
{
	std::vector<std::string> strings = {
	    "git init",
	    "git status",
	    "git add my_new_file.txt",
	    "git commit -m \"Add three files\"",
	    "git reset --soft HEAD^",
	    "git remote add origin https://github.com/heftyy/fuzzy-search.git",
	};

	HistoryItemModel history_model(nullptr);

	std::vector<HistoryItemData> history_items_data;
	for (auto i = 0; i < state.range(0); ++i)
	{
		for (const std::string& text : strings)
		{
			std::hash<std::string> hasher;
			const size_t text_hash = hasher(text);
			history_model.AddToHistory({text, text_hash, GetTimestamp()});
		}
	}

	for (const auto _ : state)
	{
		bool result = history_model.UpdateFilterPattern("add", true);
		benchmark::DoNotOptimize(result);
	}
}
//HISTORY_MODEL_BENCHMARK(BM_ModelFilterAndSort);

void BM_BasicFilterAndSort(benchmark::State& state)
{
	std::vector<std::string> base_strings = {
	    "git init",
	    "git status",
	    "git add my_new_file.txt",
	    "git commit -m \"Add three files\"",
	    "git reset --soft HEAD^",
	    "git remote add origin https://github.com/heftyy/fuzzy-search.git",
	};

	std::vector<StringWithScore> benchmark_strings;

	for (auto i = 0; i < state.range(0); ++i)
	{
		for (const std::string& str : base_strings)
		{
			benchmark_strings.push_back({str, 0});
		}
	}

	for (const auto _ : state)
	{
		auto result = FuzzySearch::Search("add", benchmark_strings.begin(), benchmark_strings.end(), &GetStringFunc, FuzzySearch::MatchMode::E_STRINGS);
		benchmark::DoNotOptimize(result);
	}
}
//HISTORY_MODEL_BENCHMARK(BM_BasicFilterAndSort);

BENCHMARK_MAIN();
