#pragma warning(disable: 26495) // Google test warnings
#include "gtest/gtest.h"
#include <btree.h>

using namespace ltc;

std::pair<std::string, std::string> make_value(const std::string& k, const std::string& v)
{
	return std::make_pair(k, v);
}


TEST(Btree, empty)
{
	Btree b(3);
	EXPECT_TRUE(b.empty());
	auto r = b.insert(make_value("1", "One"));
	EXPECT_FALSE(b.empty());
}


TEST(Btree, insert) 
{
	Btree b(3);
	auto r1 = b.insert(std::make_pair<std::string, std::string>("1", "One"));
	EXPECT_TRUE(r1.second);

	auto r2 = b.insert(std::make_pair<std::string, std::string>("1", "One"));
	EXPECT_FALSE(r2.second);

	auto r3 = b.insert(std::make_pair<std::string, std::string>("2", "Two"));
	EXPECT_TRUE(r3.second);

	auto r4 = b.insert(std::make_pair<std::string, std::string>("3", "Three"));
	EXPECT_TRUE(r4.second);

	int count{ 0 };
	for (auto v : b)
	{
		count++;
	}
	ASSERT_EQ(count, 3);
	ASSERT_EQ(b.size(), 3);
}
