#include <chrono>

#include <gtest/gtest.h>

#include <FuzzySearch.h>

TEST(TestSearchableHistory, Add)
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
