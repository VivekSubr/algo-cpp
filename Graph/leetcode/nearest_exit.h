#pragma once
#include <vector>
#include <deque>
#include <unordered_set>
#include "utils.h"
//leetcode.com/problems/nearest-exit-from-entrance-in-maze/description

bool is_move_valid(std::pair<int, int> node, const std::vector<std::vector<char>>& maze, move m,  
    std::unordered_set<std::pair<int, int>, pair_hash> set_visited)
{
    int first  = node.first;
    int second = node.second;
	switch(m)
	{
		case move::up:
			if((node.first + 1) >= maze.size()) return false;
            first = node.first + 1;
            break;
			
		case move::down:
			if((node.first - 1) < 0) return false;
            first = node.first - 1;
			break;
			
		case move::right:
			if((node.second + 1) >= maze[0].size()) return false;
			second = second + 1;
            break;
			
		case move::left:
			if((node.second - 1) < 0) return false;
			second = second - 1;
            break;
	}

    if(maze[first][second] == '+') return false;
	if(set_visited.find({first, second}) != set_visited.end()) return false;

	return true;
}

bool is_exit(std::pair<int, int> node, const std::vector<std::vector<char>>& maze)
{
	return (node.first == maze.size() - 1) || (node.second == maze[0].size() - 1 || node.first == 0 || node.second == 0);
}

int nearestExit(const std::vector<std::vector<char>>& maze, std::pair<int, int>& entrance) 
{
    std::deque<std::pair<int, int>> bfs_Q;
    std::unordered_set<std::pair<int, int>, pair_hash> set_visited;

	bfs_Q.push_back(entrance);
    set_visited.insert(entrance);
	
	int nSteps(0);
    bool at_entrance(true);
    while(!bfs_Q.empty())
    {
        auto node = bfs_Q.at(0);
        bfs_Q.pop_front();
		set_visited.insert(node);
		
        std::cout<<"Visiting node: ("<<node.first<<","<<node.second<<") at step "<<nSteps<<"\n";
		++nSteps;
		if(!at_entrance && is_exit(node, maze)) 
        {
            std::cout<<"Found exit at node: ("<<node.first<<","<<node.second<<") in "<<nSteps - 1<<" steps\n";
            break;
        }
		
		//Now, it's possible to go up, down, left and right
		if(is_move_valid(node, maze, move::up,    set_visited))    bfs_Q.push_back({node.first + 1, node.second});
		if(is_move_valid(node, maze, move::down,  set_visited))    bfs_Q.push_back({node.first - 1, node.second});
		if(is_move_valid(node, maze, move::right, set_visited))    bfs_Q.push_back({node.first, node.second + 1});
		if(is_move_valid(node, maze, move::left,  set_visited))    bfs_Q.push_back({node.first, node.second - 1});

        at_entrance = false;
	}
	
	return (nSteps - 1) ? (nSteps - 1) : -1;
}