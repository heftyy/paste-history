#include <chrono>

#include <gtest/gtest.h>

#include <HistoryItem.h>
#include <HistoryItemModel.h>

#include <QAbstractItemModelTester>

static size_t GetTimestamp()
{
	auto duration = std::chrono::system_clock::now().time_since_epoch();
	return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

static std::string GetStringFromModel(const HistoryItemModel& history_model, int row_index)
{
	return history_model.GetHistoryItemData(row_index).m_Text;
};

TEST(TestHistoryModel, TimestampOrder)
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
	auto tester = QAbstractItemModelTester(&history_model, QAbstractItemModelTester::FailureReportingMode::Fatal, nullptr);
	for (size_t i = 0; i < strings.size(); ++i)
	{
		std::hash<std::string> hasher;
		const size_t text_hash = hasher(strings[i]);
		history_model.AddToHistory({strings[i], text_hash, i});
	}

	ASSERT_EQ("git remote add origin https://github.com/heftyy/fuzzy-search.git", GetStringFromModel(history_model, 0));
	ASSERT_EQ("git reset --soft HEAD^", GetStringFromModel(history_model, 1));
	ASSERT_EQ("git commit -m \"Add three files\"", GetStringFromModel(history_model, 2));
}

TEST(TestHistoryModel, SearchOrder)
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
	auto tester = QAbstractItemModelTester(&history_model, QAbstractItemModelTester::FailureReportingMode::Fatal, nullptr);
	for (const std::string& text : strings)
	{
		std::hash<std::string> hasher;
		const size_t text_hash = hasher(text);
		history_model.AddToHistory({text, text_hash, GetTimestamp()});
	}

	ASSERT_TRUE(history_model.UpdateFilterPattern("add"));

	ASSERT_EQ("git add my_new_file.txt", GetStringFromModel(history_model, 0));
	ASSERT_EQ("git commit -m \"Add three files\"", GetStringFromModel(history_model, 1));
	ASSERT_EQ("git remote add origin https://github.com/heftyy/fuzzy-search.git", GetStringFromModel(history_model, 2));
}