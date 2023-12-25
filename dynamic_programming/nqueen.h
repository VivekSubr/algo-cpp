#pragma once
#include <vector>

//Is any Queen able to kill another in this matrix?
bool checkIfKill(const std::vector<std::vector<bool>>& matrix)
{
	std::size_t nSize = matrix.size();
	for (std::size_t i = 0; i < nSize; i++)
	{
		for (std::size_t j = 0; j < nSize; j++)
		{
			if (matrix[i][j])
			{
				/* Check horizontal kill*/
				for (std::size_t col = 0; col < nSize; col++)
				{
					if (col == j) continue;
					if (matrix[i][col]) { return true; }
				}

				for (std::size_t row = 0; row < nSize; row++)
				{
					if (row == i) continue;
					if (matrix[row][j]) { return true; }
				}
				/*********************/

				/** Check diagonal kill**/
				for (std::size_t row = 0; row < nSize; row++)
				{
					if (row == i) continue;
					for (std::size_t col = 0; col < nSize; col++)
					{
						if (row == col && matrix[row][col]) return true;
					}
				}

				/************************/
			}
		}

		return false;
	}

    return false;
}

bool do_nQueen(std::vector<std::vector<bool>>& matrix, int nQueen, int col)
{
	if (nQueen == 0) return true;
	for (std::size_t i = 0; i < matrix.size(); i++)
	{
		matrix[i][col] = true;
		if (checkIfKill(matrix)) //can any of the queens kill another?
		{ //backtrack!
			matrix[i][col] = false;
			continue;
		}
		else
		{
			nQueen--;
			if (do_nQueen(matrix, nQueen, col + 1)) return true;
		}
	}

	return false;
}