#include <chrono>
#include <thread>

#include <FuzzySearch.h>

int main(int argc, char** argv)
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

	for (int i = 0; i < 1000000; ++i)
	{
		files.insert(files.end(), original_files.begin(), original_files.end());
	}

	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	std::vector<FuzzySearch::SearchResult> results = FuzzySearch::Search("hierarchy node base", files, FuzzySearch::MatchMode::E_FILENAMES);
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

	size_t length = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	return 0;
}