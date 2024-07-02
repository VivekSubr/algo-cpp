#include "nqueen.h"
#include "dp_test.h"
#include "edit_distance.h"
#include "jump_game.h"
#include "max_subarray.h"
#include "best_time_sell_stock.h"

TEST_F(TestDP, NQueen)
{
    std::vector<std::vector<bool>> matrix = 
	{
		{ true, false, false },
		{ false, false, true },
		{ false, true, false }
	};

    ASSERT_FALSE(checkIfKill(matrix));

	matrix = //row kill
	{
	    { true, false, true },
		{ false, false, false },
		{ false, true, false }
	};

	ASSERT_TRUE(checkIfKill(matrix));

	matrix = //collumn kill
	{
		{ true, false, false },
		{ false, false, true },
		{ true, false, false }
	};

	ASSERT_TRUE(checkIfKill(matrix));

	matrix = //diagonal kill
	{
		{ true, false, false },
		{ false, true, false },
		{ false, false, true }
    };

	ASSERT_TRUE(checkIfKill(matrix));

    matrix =
	{
		{ false, false, false },
		{ false, false, false },
		{ false, false, false }
	};

	ASSERT_TRUE(do_nQueen(matrix, 3, 0));

	int nQueen(0);
	for (std::size_t i = 0; i < matrix.size(); i++)
	{
		for (std::size_t j = 0; j < matrix.size(); j++)
		{
			if (matrix[i][j]) ++nQueen;
		}
	}

	ASSERT_EQ(nQueen, 3);
	ASSERT_FALSE(checkIfKill(matrix));
}

TEST_F(TestDP, EditDistance)
{
	ASSERT_EQ(levenshtein_distance("horse", "ros"), 3);
	ASSERT_EQ(levenshtein_distance("ros", "horse"), 3);

	ASSERT_EQ(levenshtein_distance("intention", "execution"), 9);

	auto edit_dis = edit_distance("horse", "ros");
	ASSERT_EQ(edit_dis.first, 3);
	printVector(edit_dis.second);
}

TEST_F(TestDP, JumpGame)
{
    ASSERT_TRUE(JumpGame::canJump({2,3,1,1,4}));
	ASSERT_FALSE(JumpGame::canJump({3,2,1,0,4}));

	ASSERT_EQ(JumpGame::minJumps({2,3,1,1,4}), 2);
	ASSERT_EQ(JumpGame::minJumps({2,3,0,1,4}), 2);
}

TEST_F(TestDP, BestTimeSellStock)
{
	ASSERT_EQ(BestTimeSell::maxProfit({7,1,5,3,6,4}), 5);

	ASSERT_EQ(BestTimeSell::maxProfit2({7,1,5,3,6,4}),  7);
	ASSERT_EQ(BestTimeSell::maxProfit2({1,2,3,4,5}),    4);
	ASSERT_EQ(BestTimeSell::maxProfit2({7,6,4,3,1}),    0);
}