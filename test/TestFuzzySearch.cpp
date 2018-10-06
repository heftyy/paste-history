#include <chrono>

#include <gtest/gtest.h>

#include <FuzzySearch.h>

using namespace FuzzySearch;

static std::string_view GetStringFunc(std::string_view string)
{
	return string;
}

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
	    "e:/libs/otherlib/main/source/no_extension",
	};

	{
		std::vector<SearchResult> results = Search("bhn", files.begin(), files.end(), &GetStringFunc, MatchMode::E_SOURCE_FILES);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNode.cpp", results[0].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNode.h", results[1].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.cpp", results[2].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.h", results[3].m_String);
	}

	{
		std::vector<SearchResult> results = Search("bhnl", files.begin(), files.end(), &GetStringFunc, MatchMode::E_SOURCE_FILES);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.cpp", results[0].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.h", results[1].m_String);
	}

	{
		std::vector<SearchResult> results = Search("hierarchy node base", files.begin(), files.end(), &GetStringFunc, MatchMode::E_SOURCE_FILES);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNode.cpp", results[0].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNode.h", results[1].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.cpp", results[2].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.h", results[3].m_String);
	}

	{
		std::vector<SearchResult> results = Search("cmakelists node", files.begin(), files.end(), &GetStringFunc, MatchMode::E_SOURCE_FILES);
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
	    "e:/libs/otherlib/main/source/no_extension",
	};

	{
		std::vector<SearchResult> results = Search("bhn", files.begin(), files.end(), &GetStringFunc, MatchMode::E_FILENAMES);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNode.h", results[0].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNode.cpp", results[1].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.h", results[2].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.cpp", results[3].m_String);
	}

	{
		std::vector<SearchResult> results = Search("node loader", files.begin(), files.end(), &GetStringFunc, MatchMode::E_FILENAMES);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.h", results[0].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.cpp", results[1].m_String);
	}

	{
		std::vector<SearchResult> results = Search("hierarchy node base", files.begin(), files.end(), &GetStringFunc, MatchMode::E_FILENAMES);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNode.h", results[0].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNode.cpp", results[1].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.h", results[2].m_String);
		ASSERT_EQ("e:/libs/nodehierarchy/main/source/BaseHierarchyNodeLoader.cpp", results[3].m_String);
	}

	{
		std::vector<SearchResult> results = Search("cmakelists", files.begin(), files.end(), &GetStringFunc, MatchMode::E_SOURCE_FILES);
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
		std::vector<SearchResult> results = Search("git", files.begin(), files.end(), &GetStringFunc, MatchMode::E_STRINGS);
		ASSERT_EQ("git init", results[0].m_String);
		ASSERT_EQ("git status", results[1].m_String);
	}

	{
		std::vector<SearchResult> results = Search("add", files.begin(), files.end(), &GetStringFunc, MatchMode::E_STRINGS);
		ASSERT_EQ("git add my_new_file.txt", results[0].m_String);
		ASSERT_EQ("git commit -m \"Add three files\"", results[1].m_String);
		ASSERT_EQ("git remote add origin https://github.com/heftyy/fuzzy-search.git", results[2].m_String);
	}

	{
		std::vector<SearchResult> results = Search("reset", files.begin(), files.end(), &GetStringFunc, MatchMode::E_STRINGS);
		ASSERT_EQ("git reset --soft HEAD^", results[0].m_String);
	}
}
