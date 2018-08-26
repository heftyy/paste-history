#include <chrono>

#include <gtest/gtest.h>

#include <FuzzySearch.h>

TEST(FuzzySearchTest, SourceFiles)
{
	std::vector<std::string> files = {
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

	{
		std::vector<FuzzySearch::SearchResult> results = FuzzySearch::Search("bhn", files, FuzzySearch::MatchMode::E_SOURCE_FILES);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNode.cpp", results[0].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNode.h", results[1].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.cpp", results[2].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.h", results[3].m_String);
	}

	{
		std::vector<FuzzySearch::SearchResult> results = FuzzySearch::Search("bhnl", files, FuzzySearch::MatchMode::E_SOURCE_FILES);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.cpp", results[0].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.h", results[1].m_String);
	}

	{
		std::vector<FuzzySearch::SearchResult> results = FuzzySearch::Search("hierarchy node base", files, FuzzySearch::MatchMode::E_SOURCE_FILES);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNode.cpp", results[0].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNode.h", results[1].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.cpp", results[2].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.h", results[3].m_String);
	}

	{
		std::vector<FuzzySearch::SearchResult> results = FuzzySearch::Search("cmakelists node", files, FuzzySearch::MatchMode::E_SOURCE_FILES);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/CMakeLists.txt", results[0].m_String);
	}
}

TEST(FuzzySearchTest, Filenames)
{
	std::vector<std::string> files = {
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

	{
		std::vector<FuzzySearch::SearchResult> results = FuzzySearch::Search("bhn", files, FuzzySearch::MatchMode::E_FILENAMES);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNode.h", results[0].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNode.cpp", results[1].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.h", results[2].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.cpp", results[3].m_String);
	}

	{
		std::vector<FuzzySearch::SearchResult> results = FuzzySearch::Search("node loader", files, FuzzySearch::MatchMode::E_FILENAMES);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.h", results[0].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.cpp", results[1].m_String);
	}

	{
		std::vector<FuzzySearch::SearchResult> results = FuzzySearch::Search("hierarchy node base", files, FuzzySearch::MatchMode::E_FILENAMES);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNode.h", results[0].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNode.cpp", results[1].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.h", results[2].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.cpp", results[3].m_String);
	}

	{
		std::vector<FuzzySearch::SearchResult> results = FuzzySearch::Search("cmakelists", files, FuzzySearch::MatchMode::E_SOURCE_FILES);
		ASSERT_EQ("e:/libs/otherlib/main/source/CMakeLists.txt", results[0].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/CMakeLists.txt", results[1].m_String);
	}
}

TEST(FuzzySearchTest, Strings)
{
	std::vector<std::string> files = {
	    "git init",
	    "git status",
	    "git add my_new_file.txt",
	    "git commit -m \"Add three files\"",
	    "git reset --soft HEAD^",
	    "git remote add origin https://github.com/heftyy/fuzzy-search.git",
	};

	{
		std::vector<FuzzySearch::SearchResult> results = FuzzySearch::Search("git", files, FuzzySearch::MatchMode::E_STRINGS);
		ASSERT_EQ("git init", results[0].m_String);
		ASSERT_EQ("git status", results[1].m_String);
	}

	{
		std::vector<FuzzySearch::SearchResult> results = FuzzySearch::Search("add", files, FuzzySearch::MatchMode::E_STRINGS);
		ASSERT_EQ("git add my_new_file.txt", results[0].m_String);
		ASSERT_EQ("git commit -m \"Add three files\"", results[1].m_String);
		ASSERT_EQ("git remote add origin https://github.com/heftyy/fuzzy-search.git", results[2].m_String);
	}

	{
		std::vector<FuzzySearch::SearchResult> results = FuzzySearch::Search("reset", files, FuzzySearch::MatchMode::E_STRINGS);
		ASSERT_EQ("git reset --soft HEAD^", results[0].m_String);
	}
}

TEST(FuzzySearchTest, Benchmark)
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

	std::vector<std::string> files;

	for (int i = 0; i < 10000; ++i)
	{
		files.insert(files.end(), original_files.begin(), original_files.end());
	}

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	std::vector<FuzzySearch::SearchResult> results = FuzzySearch::Search("hierarchy node base", files, FuzzySearch::MatchMode::E_FILENAMES);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	size_t length = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

#ifdef NDEBUG
	#define BENCHMARK_MAX_MS 3000
#else
	#define BENCHMARK_MAX_MS 30000
#endif

	ASSERT_LT(length, BENCHMARK_MAX_MS);
}