#pragma once

int find_max_detonate(const std::unordered_map<int, std::vector<int>>& bomb_adj_list)
{
	int max;
	for(auto bombs : bomb_adj_list)
	{
		int cur(0);
		std::queue<int> bfs_Q;
		std::unordered_set<int> set_visited;
		
		bfs_Q.push_back(bombs.at(0));
		while(!bombs.empty())
		{
			int bomb = bfs_Q.front();
			bfs_Q.pop_front();
			
			set_visited.insert(bomb);
			++cur;
			
			for(auto next_bomb : bomb_adj_list.at(bomb))
			{
				if(set_visited.find(next_bomb) == set_visited.end())
				{
					bfs_Q.push_back(next_bomb);
				}
			}				
			
			max = std::max(max, cur);
		}
	}
	
	return 1 + max;
}

//leetcode.com/problems/detonate-the-maximum-bombs/description
int maximumDetonation(std::vector<std::vector<int>>& bombs) 
{
/*
	Build this as a Directed Graph... if a bomb has another bomb in it's radius, then that's a connection.
*/
    
	//with nodes being indexed by bombs vector index
	std::unordered_map<int, std::vector<int>> bomb_adj_list;
	
	for(size_t i=0; i<bombs.size(); i++)
	{
		int x = bombs.at(i).at(0);
		int y = bombs.at(i).at(1);
		int r = bombs.at(i).at(2);
		for(size_t j=0; j<bombs.size(); j++)
		{
			//the formula to check if a point (x,y) lies inside or on a circle centered at (i,j) with radius r is:
			//(x-i)^2 + (y-j)^2 < r^2
			
			if(i==j) return;
			
			int i = bombs.at(i).at(0);
			int j = bombs.at(j).at(1);
			
			if((x-i)*(x-i) + (y-j)*(y-j) < r*r) 
			{ //detonate! Add connection
			  bomb_adj_list[i].push_back(j);
			}
		}
	}
	
	return find_max_detonate(bomb_adj_list);
}