#pragma once
#include <vector>

//leetcode.com/problems/snakes-and-ladders/

std::vector<int> next_range(int curr, int n)
{
    std::vector<int> ret;
    for(int i= (curr + 1); i <= std::min(curr + 6, n * n); i++) 
    {
        ret.push_back(i);
    }
    
    return ret;
}

int get_next_pos(int cur, int next, const std::vector<std::vector<int>>& board)
{
    //start position is last row, first col,
    int rows = cur / board.size();
    int cols = cur % board.size();

    int cur_row = board.size() - 1 - rows;
    int pos = board[cur_row][cols == 0 ? 0 : cols - 1];

    if (pos != -1) return pos;
    return next;
}

int do_game(int cur, int count, const std::vector<std::vector<int>>& board)
{
    auto next_arr = next_range(cur, board.size());
    auto last_index = board.size() * board.size();

    for(auto next : next_arr)
    {
        next = get_next_pos(cur, next, board);
        
        if(next == board.size() * board.size()) return count;
        int ret = do_game(next - 1, count + 1, board);
        if(ret != -1) return ret;
    }

    return -1;
}

int snakesAndLadders(std::vector<std::vector<int>>& board) 
{
    return do_game(1, 0, board);
}