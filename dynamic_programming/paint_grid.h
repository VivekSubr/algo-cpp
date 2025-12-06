#pragma once
#include <vector>
#include <unordered_set>

/*
    You are given two integers m and n. Consider an m x n grid where each cell is initially white. 
    You can paint each cell red, green, or blue. All cells must be painted.

    Return the number of ways to color the grid with no two adjacent cells having the same color. 
    Since the answer can be very large, return it modulo 10^9 + 7.
*/

enum class Color {
    RED,
    GREEN,
    BLUE
};

std::pair<Color, Color> get_other_colors(Color color)
{
    switch(color)
    {
        case Color::RED:
            return {Color::GREEN, Color::BLUE};
        case Color::GREEN:
            return {Color::RED, Color::BLUE};
        case Color::BLUE:
            return {Color::RED, Color::GREEN};
    }
}

std::string stringify(int row, int col)
{
    return std::to_string(row) + "-" + std::to_string(col);
}

int do_color(int row, int col, int m, int n, int count, Color color, std::unordered_set<std::string>& visited)
{ //so, (row, col) is marked color... hence the adjacent cells can be the other two
    
    std::string key = stringify(row, col);
    if(visited.find(key) != visited.end()) return 0;
    visited.insert(key);

    auto other_col = get_other_colors(color);
    
  /* if(row - 1 >= 0 && visited.find(stringify(row - 1, col)) == visited.end())
    {
        count += do_color(row - 1, col, m, n, count, other_col.first,  visited) +
                 do_color(row - 1, col, m, n, count, other_col.second, visited);
    }

    if(col - 1 >= 0 && visited.find(stringify(row, col - 1)) == visited.end())
    {
        count += do_color(row, col - 1, m, n, count, other_col.first,  visited) +
                 do_color(row, col - 1, m, n, count, other_col.second, visited);
    }

    if(row + 1 < m && visited.find(stringify(row + 1, col)) == visited.end())
    {
        count += do_color(row + 1, col, m, n, count, other_col.first,  visited) +
                 do_color(row + 1, col, m, n, count, other_col.second, visited);
    }

    if(col + 1 < n && visited.find(stringify(row, col + 1)) == visited.end())
    {
        count += do_color(row, col + 1, m, n, count, other_col.first,  visited) + 
                 do_color(row, col + 1, m, n, count, other_col.second, visited);
    } */

    return count + 1;
}

int colorTheGrid(int m, int n) 
{ // m x n grid
   //three maps for three starting points
   std::unordered_set<std::string> visited_red, visited_green, visited_blue;

   return 
    do_color(0, 0, m, n, 0, Color::RED,   visited_red) + 
    do_color(0, 0, m, n, 0, Color::GREEN, visited_green) + 
    do_color(0, 0, m, n, 0, Color::BLUE,  visited_blue); 
}