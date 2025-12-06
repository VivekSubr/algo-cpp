#pragma once
#include <vector>
#include <unordered_set>
#include <tuple>
#include <deque>
#include "utils.h"
//leetcode.com/problems/number-of-islands/description

std::vector<std::tuple<size_t, size_t, char>> get_edges(const std::vector<std::vector<char>>& grid)
{
    std::vector<std::tuple<size_t, size_t, char>> ret;
    for(size_t i=0; i<grid.size(); i++)
    {
       for(size_t j=0; j<grid[i].size(); j++)
       {
            if(grid[i][j] == '1') 
            {
               ret.push_back({i, j, grid[i][j]});
            }
       }
    }

    return ret;
}
	
//returns root of node's disjoint set
size_t find(size_t node, std::vector<size_t>& parentArr)
{
    size_t ret = node; //If completely disjoint, node is own parent
    while(ret != parentArr[ret])
    { //go up till we find the root
       ret = parentArr[ret]; 
    }

    return ret;
}

bool do_union(size_t n1, size_t n2, std::vector<size_t>& rankArr, std::vector<size_t>& parentArr)
{
	//Find the parents
    size_t p1 = find(n1, parentArr);
    size_t p2 = find(n2, parentArr);

    if(p1 == p2) return false; //already in same connected component

    if(rankArr[p2] > rankArr[p1]) //prefer to union with the larger component
    {
        parentArr[p2] = p1;
        rankArr[p2]++;
    }
    else
    {
       parentArr[p1] = p2;
       rankArr[p1]++;
    }

    return true;
}

size_t count_connected_components(const std::vector<std::vector<char>>& grid, std::vector<size_t>& rankArr, std::vector<size_t>& parentArr)
{
	size_t nCount = grid.size();   
    for(auto& edge : get_edges(grid))
    {
       if(do_union(std::get<0>(edge), std::get<1>(edge), rankArr, parentArr))
       {
          --nCount;
       }
    }

    return nCount;
}

size_t numIslands(const std::vector<std::vector<char>>& grid) 
{
    std::vector<size_t> parentArr(grid.size(), -1); //index - value represents one edge
    std::vector<size_t> rankArr(grid.size(), 0);   //'rank' of a node is the size of the connected component it belongs to

	for(size_t i=0; i<grid.size(); i++)
    {
       parentArr[i] = i; //Set parent to itself to begin with
    }
	
	return count_connected_components(grid, rankArr, parentArr);
}

//Alternative BFS based solution

bool move_has_land(const std::pair<size_t, size_t>& index, 
    const std::vector<std::vector<char>>& grid, 
    std::unordered_set<std::pair<size_t, size_t>, pair_hash>& visited_set,
    move m)
{
    // Use signed temps to avoid size_t underflow when moving up/left
    long i = static_cast<long>(index.first);
    long j = static_cast<long>(index.second);

    switch (m) {
        case move::up:    --i; break;
        case move::down:  ++i; break;
        case move::right: ++j; break;
        case move::left:  --j; break;
    }

    // Bounds check on signed values, then convert to size_t
    if (i < 0 || j < 0) return false;
    if (i >= static_cast<long>(grid.size()) || j >= static_cast<long>(grid[0].size())) return false;

    size_t ui = static_cast<size_t>(i);
    size_t uj = static_cast<size_t>(j);

    if (grid[ui][uj] == '0') return false;
    if (visited_set.find({ui, uj}) != visited_set.end()) return false;

    return true;

}

void do_island_bfs(std::pair<size_t, size_t> index, const std::vector<std::vector<char>>& grid, 
                     std::unordered_set<std::pair<size_t, size_t>, pair_hash>& visited_set)
{
	std::deque<std::pair<size_t, size_t>> bfs_Q; 
	
	bfs_Q.push_back(index);	
	while(!bfs_Q.empty())
	{
		auto index = bfs_Q.at(0);
		bfs_Q.pop_front();
		
		if(move_has_land(index, grid, visited_set, move::up))
		{
			bfs_Q.push_back({index.first - 1, index.second});
            visited_set.insert({index.first - 1, index.second});
		}
		
		if(move_has_land(index, grid, visited_set, move::down))
		{
			bfs_Q.push_back({index.first + 1, index.second});
            visited_set.insert({index.first + 1, index.second});
		}
		
		if(move_has_land(index, grid, visited_set, move::right))
		{
			bfs_Q.push_back({index.first, index.second + 1});
            visited_set.insert({index.first, index.second + 1});
		}
		
		if(move_has_land(index, grid, visited_set, move::left))
		{
			bfs_Q.push_back({index.first, index.second - 1});
            visited_set.insert({index.first, index.second - 1});
		}
	}
}

//So, this grid is a matrix of '1' and '0'... start by bfs at (0,0) if it's '1' or closest, mark all found and repeat with un-marked till
//everything is covered
size_t numIslands_bfs(const std::vector<std::vector<char>>& grid) 
{
	size_t nIslands(0);

	std::unordered_set<std::pair<size_t, size_t>, pair_hash> visited_set;
	for(size_t i=0; i<grid.size(); i++)
	{
		for(size_t j=0; j<grid[0].size(); j++)
		{
			if(grid[i][j] == '1' && visited_set.find({i, j}) == visited_set.end())
			{
				do_island_bfs({i, j}, grid, visited_set);
				++nIslands;
			}
		}
	}

    return nIslands;
}

//leetcode.com/problems/number-of-closed-islands/description
size_t numClosedIslands(const std::vector<std::vector<int>>& grid) 
{
    return 0;
}