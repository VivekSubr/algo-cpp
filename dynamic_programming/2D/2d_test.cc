#include "2d_test.h"
#include "count_subseq.h"
#include "edit_distance.h"

TEST_F(Test2D, CountSubSeq)
{
    std::vector<std::tuple<std::string, std::string, int>> tests = {
        {"caaat", "cat", 3},
        {"rabbbit", "rabbit", 3},
        {"babgbag", "bag",    5}
    };

    for(const auto& [s, t, expected] : tests)
    {
        std::cout<<"Testing s = "<<s<<", t = "<<t<<"\n";
        ASSERT_EQ(count_subseq(s, t), expected);
        std::cout<<"\n";
    }
}

TEST_F(Test2D, EditDistance)
{
	ASSERT_EQ(levenshtein_distance("horse", "ros"), 3);
	ASSERT_EQ(levenshtein_distance("ros", "horse"), 3);

	ASSERT_EQ(levenshtein_distance("intention", "execution"), 9);

	auto edit_dis = edit_distance("horse", "ros");
	ASSERT_EQ(edit_dis.first, 3);
	printVector(edit_dis.second);
}