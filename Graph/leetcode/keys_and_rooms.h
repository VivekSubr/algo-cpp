#pragma once
#include <vector>
#include <unordered_set>
//leetcode.com/problems/keys-and-rooms/description

void do_canVisit(int node, const std::vector<std::vector<int>>& rooms, std::unordered_set<int>& set_visited)
{
	if(set_visited.find(node) != set_visited.end()) return;

	set_visited.insert(node);
	for(int room : rooms[node])
	{
		if(set_visited.find(room) != set_visited.end()) continue;
		do_canVisit(room, rooms, set_visited);
	}
}

//Directed list representation... with 'key' being direction to other room/node
bool canVisitAllRooms(std::vector<std::vector<int>>& rooms) 
{	
//All rooms locked expect room 0... hence do dfs from 0, and it set visited contains all nodes, return true

	std::unordered_set<int> set_visited;
	do_canVisit(0, rooms, set_visited);

	return set_visited.size() == rooms.size();
}