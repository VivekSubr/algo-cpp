#include "graph_test.h"
#include "number_provinces.h"
#include "eventual_safe_state.h"
#include "keys_and_rooms.h"
#include "nearest_exit.h"
#include "number_islands.h"
#include "network_delay_time.h"
#include <tuple>

TEST_F(TestGraph, NumberProvinces)
{
   std::vector<std::tuple<std::vector<std::vector<int>>, int>> test_cases = 
   {
       std::make_tuple(std::vector<std::vector<int>>{{1,1,0},{1,1,0},{0,0,1}}, 2),
       std::make_tuple(std::vector<std::vector<int>>{{1,0,0},{0,1,0},{0,0,1}}, 3),
       std::make_tuple(std::vector<std::vector<int>>{{1,1,0},{1,1,1},{0,1,1}}, 1),
   };

    for(auto& [isConnected, expected_nProvinces] : test_cases)
    {
        int nProvinces = number_provinces(isConnected);
        EXPECT_EQ(nProvinces, expected_nProvinces);
    }
}

TEST_F(TestGraph, EventualSafeNodes)
{
    std::vector<std::tuple<std::vector<std::vector<int>>, std::vector<int>>> test_cases = 
    {
         std::make_tuple(std::vector<std::vector<int>>{{1,2},{2,3},{5},{0},{5},{},{}}, std::vector<int>{2,4,5,6}),
         std::make_tuple(std::vector<std::vector<int>>{{1,2,3,4},{1,2},{3,4},{0,4},{}}, std::vector<int>{4}),
    };
    
     for(auto& [graph, expected_safe_nodes] : test_cases)
     {
          std::vector<int> safe_nodes = eventualSafeNodes(graph);
          EXPECT_EQ(safe_nodes, expected_safe_nodes);
     }
}

TEST_F(TestGraph, KeysAndRooms)
{
    std::vector<std::tuple<std::vector<std::vector<int>>, bool>> test_cases = 
    {
        std::make_tuple(std::vector<std::vector<int>>{{1},{2},{3},{}}, true),
        std::make_tuple(std::vector<std::vector<int>>{{1,3},{3,0,1},{2},{0}}, false),
    };

    for(auto& [rooms, expected] : test_cases)
    {
        bool result = canVisitAllRooms(rooms);
        EXPECT_EQ(result, expected);
    }
}

TEST_F(TestGraph, NearestExit)
{
    std::vector<std::tuple<std::vector<std::vector<char>>, std::pair<int, int>, int>> test_cases = 
    {
        std::make_tuple(std::vector<std::vector<char>>{{'+','+','.','+'},{'.','.','.','+'},{'+','+','+','.'}}, std::make_pair(1,2), 1),
        std::make_tuple(std::vector<std::vector<char>>{{'+','+','+'},{'.','.','.'},{'+','+','+'}}, std::make_pair(1,0), 2),
        std::make_tuple(std::vector<std::vector<char>>{{'.','+'}}, std::make_pair(0,0), -1),
    };

    for(auto& [maze, entrance, expected_steps] : test_cases)
    {
        int nSteps = nearestExit(maze, entrance);
        EXPECT_EQ(nSteps, expected_steps);
    }
}

TEST_F(TestGraph, NumberIslands)
{
    std::vector<std::tuple<std::vector<std::vector<char>>, size_t>> test_cases = 
    {
        std::make_tuple(std::vector<std::vector<char>>{{'1','1','0','0','0'},{'1','1','0','0','0'},{'0','0','1','0','0'},{'0','0','0','1','1'}}, 3),
        std::make_tuple(std::vector<std::vector<char>>{{'1','1','0','1','0'},{'1','0','0','1','0'},{'0','0','1','0','1'},{'1','0','0','1','1'}}, 5),
    };

    for(auto& [grid, expected_nIslands] : test_cases)
    {
        size_t nIslands = numIslands_bfs(grid);
        EXPECT_EQ(nIslands, expected_nIslands);
    }
}

TEST_F(TestGraph, NetworkDelayTime)
{
    std::vector<std::tuple<std::vector<std::vector<int>>, int, int, int>> test_cases = 
    {
        std::make_tuple(std::vector<std::vector<int>>{{2,1,1},{2,3,1},{3,4,1}}, 4, 2, 2),
        std::make_tuple(std::vector<std::vector<int>>{{1,2,1}}, 2, 1, 1),
        std::make_tuple(std::vector<std::vector<int>>{{1,2,1}}, 2, 2, -1),
    };

    for(auto& [times, nNodes, k, expected_time] : test_cases)
    {
        int time = networkDelayTime(times, nNodes, k);
        EXPECT_EQ(time, expected_time);
    }
}
