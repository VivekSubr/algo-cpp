#include "nqueen.h"
#include "dp_test.h"

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