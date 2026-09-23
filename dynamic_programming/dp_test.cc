#include "nqueen.h"
#include "dp_test.h"
#include "jump_game.h"
#include "max_subarray.h"
#include "best_time_sell_stock.h"
#include "word_search.h"
#include "snakes_and_ladders.h"
#include "paint_grid.h"

// make visualize FILE=max_subarray.h FUNCTION=minSubArrayWithTarget ALGOVIZ_ARGS='--test TestDP.MinSubArrayWithTarget'
TEST_F(TestDP, MinSubArrayWithTarget)
{
	auto sol1 = minSubArrayWithTarget({2,3,1,2,4,3}, 7);
	ASSERT_EQ(sol1.first,  4);
	ASSERT_EQ(sol1.second, 5);

	auto sol2 = minSubArrayWithTarget({5,1,3,5,10,7,4,9,2,8}, 15);
	ASSERT_EQ(sol2.first,  3);
	ASSERT_EQ(sol2.second, 4);
}

// make visualize FILE=max_subarray.h FUNCTION=minWindowSubstring ALGOVIZ_ARGS='--test TestDP.MinWindowSubstring'
TEST_F(TestDP, MinWindowSubstring)
{
	ASSERT_EQ(minWindowSubstring("ADOBECODEBANC", "ABC"), "BANC");
	ASSERT_EQ(minWindowSubstring("a", "a"), "a");
	ASSERT_EQ(minWindowSubstring("a", "aa"), "");
	ASSERT_EQ(minWindowSubstring("a", "b"),  "");
	//ASSERT_EQ(minWindowSubstring("ab", "a"), "a");
}

// make visualize FILE=nqueen.h ALGOVIZ_ARGS='--test TestDP.NQueen'
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

// make visualize FILE=jump_game.h ALGOVIZ_ARGS='--test TestDP.JumpGame'
TEST_F(TestDP, JumpGame)
{
    ASSERT_TRUE(JumpGame::canJump({2,3,1,1,4}));
	ASSERT_FALSE(JumpGame::canJump({3,2,1,0,4}));

	ASSERT_EQ(JumpGame::minJumps({2,3,1,1,4}), 2);
	ASSERT_EQ(JumpGame::minJumps({2,3,0,1,4}), 2);
}

// make visualize FILE=best_time_sell_stock.h ALGOVIZ_ARGS='--test TestDP.BestTimeSellStock'
TEST_F(TestDP, BestTimeSellStock)
{
	ASSERT_EQ(BestTimeSell::maxProfit({7,1,5,3,6,4}),  5);
	ASSERT_EQ(BestTimeSell::maxProfit2({7,1,5,3,6,4}), 7);
	ASSERT_EQ(BestTimeSell::maxProfit2({1,2,3,4,5}),   4);
	ASSERT_EQ(BestTimeSell::maxProfit2({7,6,4,3,1}),   0);

	ASSERT_EQ(BestTimeSell::maxProfitK(2, {2,4,1}),   2);
	//ASSERT_EQ(BestTimeSell::maxProfitK(2, {3,2,6,5,0,3}),   7);
}

// make visualize FILE=word_search.h FUNCTION=WordSearch::exists ALGOVIZ_ARGS='--test TestDP.WordSearch'
TEST_F(TestDP, WordSearch)
{
	std::vector<std::vector<char>> board = {
		{'A','B','C','E'},
		{'S','F','C','S'},
		{'A','D','E','E'}
	};

	ASSERT_TRUE(WordSearch::exists(board, "ABCCED"));
} 

// make visualize FILE=snakes_and_ladders.h FUNCTION=snakesAndLadders ALGOVIZ_ARGS='--test TestDP.SnakesLadders'
TEST_F(TestDP, SnakesLadders)
{
	std::vector<std::vector<int>> board = {
		{-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1},
		{-1,-1,-1,-1,-1,-1},
		{-1,35,-1,-1,13,-1},
		{-1,-1,-1,-1,-1,-1},
		{-1,15,-1,-1,-1,-1} //starting position, 1, is r = 5, c = 0
	};

	ASSERT_EQ(get_next_pos(1, 1, board), 15);
	ASSERT_EQ(get_next_pos(1, 2, board), 3);

	ASSERT_EQ(snakesAndLadders(board), 4);

	board = {
		{-1,-1},
		{-1,3}
	};

	ASSERT_EQ(snakesAndLadders(board), 1);
} 

// make visualize FILE=paint_grid.h FUNCTION=colorTheGrid ALGOVIZ_ARGS='--test TestDP.PaintGrid'
TEST_F(TestDP, PaintGrid)
{
	std::vector<std::tuple<int, int, int>> tests = {
		{1, 1, 3},
		{1, 2, 6},
		{2, 2, 18},
		{3, 3, 246},
		{3, 4, 594},
		{4, 4, 1536}
	};

	for(auto [m, n, expected] : tests)
	{
		ASSERT_EQ(colorTheGrid(m, n), expected) << " for m: " << m << " n: " << n;
	}
}