#pragma once
#include <vector>
#include <algorithm>

enum direction
{
	right, left, up, down
};

void move_spiral(direction d, size_t& row, size_t& col)
{
	switch(d)
	{
		case direction::right:
			row++;
			break;
			
		case direction::left:
			row--;
			break;
			
		case direction::up:
			col--;
			break;
			
		case direction::down:
			col++;
			break;
	}
}

//Given an m x n matrix, return all elements of the matrix in spiral order.
std::vector<int> spiral_matrix(std::vector<std::vector<int>>& matrix)
{
    std::vector<int> ret;

    //basically, go (0,0) to (0,n-1), (1,n-1) to (m-1,n-1), (m-1,n-2) to (m-1,0), (m-2,0) to (1,0)
    //then, (1,1) to (1,n-2), (2,n-2) to (m-2,n-2), (m-2,n-3) to (m-2,1), (m-3,1) to (2,1)
    //and so on...
	
	//so, let's put in four indices to rotate on
	size_t row_start=0, row_end=matrix.size()-1, col_start=0, col_end=matrix[0].size();
	
	size_t nCount = matrix.size() * matrix[0].size();
	size_t row(0), col(0);
	direction cur_dir = direction::right;
	while(nCount > 0)
	{
		ret.push_back(matrix.at(row).at(col));
		move_spiral(cur_dir, row, col);
		
		if(row == row_start && col == col_start)
		{ //reached left top of spiral
		  cur_dir = direction::right;
		  row_start++;
		}
		else if(row == row_start && col == col_end)
		{ //reached right top of spiral
		  cur_dir = direction::down;
		  row_end--;
		}
		else if(row == row_end && col == col_end)
		{ //reached right bottom of spiral
		  cur_dir = direction::left;
		  col_end--;
		}
		else if(row == row_start && col == col_end)
		{//reached left bottom of spiral
		   cur_dir = direction::up;
		   col_start++;
		}
		
		--nCount;
	}
	
	return ret;
}

//leetcode.com/problems/maximal-square/description/
int maximalSquare(std::vector<std::vector<char>>& matrix) 
{
/*
	define, dp[i][j] = side length of the largest square ending at cell (i, j)
    And keep track of side,
	If matrix[i][j] == '1':
		dp[i][j] = 1 + min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1])
		
		Here, dp[i][j] is min of the distance to the other three corners... hence these four form a square
		To form a square at (i,j), you need:
			A square above (dp[i-1][j])
			A square to the left (dp[i][j-1])
			A square diagonally (dp[i-1][j-1])
        The smallest of these determines the largest possible square you can extend.
	
	Else:
		dp[i][j] = 0
*/

    std::vector<std::vector<int>> dp(matrix.size() + 1, std::vector<int>(matrix[0].size() + 1, 0));
    int maxSide = 0;
	
	for (size_t i = 1; i < matrix.size(); ++i) 
	{
        for (size_t j = 1; j < matrix[0].size(); ++j) 
		{
            if (matrix[i-1][j-1] == '1') 
			{
                dp[i][j] = 1 + std::min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]});
                maxSide = std::max(maxSide, dp[i][j]);
            }
        }
    }
    
	return maxSide * maxSide;
}

void move_increasing(const std::vector<std::vector<int>>& matrix, int i, int j, int cur_path, int& max_path)
{
	if(i < 0 || i >= matrix.size() || j < 0 || j >= matrix[0].size()) return;

	++cur_path;
	if(i+1 < matrix.size() && matrix[i+1][j] > matrix[i][j])
	{
		move_increasing(matrix, i+1, j, cur_path, max_path); 
	}
	
	if(i > 0 && matrix[i-1][j] > matrix[i][j])
	{
		move_increasing(matrix, i-1, j, cur_path, max_path); 
	}
	
	if(j+1 < matrix[0].size() && matrix[i][j+1] > matrix[i][j])
	{
		move_increasing(matrix, i, j+1, cur_path, max_path); 
	}
	
	if(j > 0 && matrix[i][j-1] > matrix[i][j])
	{
		move_increasing(matrix, i, j-1, cur_path, max_path); 
	}
	
	max_path = std::max(max_path, cur_path);
}

//leetcode.com/problems/longest-increasing-path-in-a-matrix/description/
int longestIncreasingPath(std::vector<std::vector<int>>& matrix) 
{
   int max_path(0);
   for(size_t i=0; i<matrix.size(); i++)
   {
	   for(size_t j=0; j<matrix[0].size(); j++)
	   {
		   move_increasing(matrix, static_cast<int>(i), static_cast<int>(j), 0, max_path);
	   }
   }
   
   return max_path;
}