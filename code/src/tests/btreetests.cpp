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
	auto r1 = b.insert(make_value("1", "One"));
	EXPECT_TRUE(r1.second);

	auto r2 = b.insert(make_value("1", "One"));
	EXPECT_FALSE(r2.second);

	auto r3 = b.insert(make_value("2", "Two"));
	EXPECT_TRUE(r3.second);

	auto r4 = b.insert(make_value("3", "Three"));
	EXPECT_TRUE(r4.second);

	int count{ 0 };
	for (auto v : b)
		count++;

	ASSERT_EQ(count, 3);
	ASSERT_EQ(b.size(), 3);
}

TEST(Btree, iterate)
{
	Btree b(3);
	auto r1 = b.insert(make_value("2", "Two"));
	auto r2 = b.insert(make_value("1", "One"));
	auto r3 = b.insert(make_value("3", "Three"));

	std::string s;
	for (auto v : b)
	{
		s += v.first;
	}
	ASSERT_EQ(s, "123");
}

TEST(Btree, InsertWithSplitLast)
{
	Btree b(3);
	auto r1 = b.insert(make_value("2", "Two"));
	auto r2 = b.insert(make_value("1", "One"));
	auto r3 = b.insert(make_value("3", "Three"));
	auto r4 = b.insert(make_value("4", "Four"));

	ASSERT_EQ(b.size(), 4);
}

TEST(Btree, IterateWithSplit)
{
	Btree b(3);
	auto r1 = b.insert(make_value("2", "Two"));
	auto r2 = b.insert(make_value("1", "One"));
	auto r3 = b.insert(make_value("3", "Three"));
	auto r4 = b.insert(make_value("4", "Four"));

	std::string s;
	for (auto v : b)
	{
		s += v.first;
	}
	ASSERT_EQ(s, "1234");
}
