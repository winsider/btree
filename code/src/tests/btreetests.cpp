#pragma warning(disable: 26495) // Google test warnings
#include "gtest/gtest.h"
#include <btree.h>

using namespace ltc;

TEST(Btree, Insert) 
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
}
