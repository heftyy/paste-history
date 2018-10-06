#include <chrono>

#include <gtest/gtest.h>

#include <SearchableHistory.h>

static size_t GetTimestamp()
{
	auto duration = std::chrono::system_clock::now().time_since_epoch();
	return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

TEST(TestSearchableHistory, Add)
{
	SearchableHistory searchable_history;

	for (int i = 0; i < 5; ++i)
	{
		CopiedText ct;
		ct.set_text("my test " + std::to_string(i));
		ct.set_expire_timestamp(GetTimestamp());
		searchable_history.AddText(ct);
	}

	gsl::span<CopiedText> copied_history = searchable_history.GetAll();

	ASSERT_EQ(copied_history.size(), 5);

	//ASSERT_EQ(copied_history[0].text(), "my test 0");
	//ASSERT_EQ(copied_history[1].text(), "my test 1");
	//ASSERT_EQ(copied_history[2].text(), "my test 2");
	//ASSERT_EQ(copied_history[3].text(), "my test 3");
	//ASSERT_EQ(copied_history[4].text(), "my test 4");
}
