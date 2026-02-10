#pragma once
#include <vector>
#include <unordered_map>

/*
    In a town, there are n people labeled from 1 to n.
     There is a rumor that one of these people is secretly the town judge.

    If the town judge exists, then:
        1. The town judge trusts nobody.
        2. Everybody (except for the town judge) trusts the town judge.
        3. There is exactly one person that satisfies properties 1 and 2.

    You are given an array trust where trust[i] = [ai, bi] representing that the person labeled 
    ai trusts the person labeled bi.

    Return the label of the town judge if the town judge exists and can be identified, 
    or return -1 otherwise.
*/

std::unordered_map<int, std::vector<int>> build_adjList(std::vector<std::vector<int>>& trust)
{
    std::unordered_map<int, std::vector<int>> adjList;
    for(const auto& edge : trust) {
        int from = edge[0];
        int to   = edge[1];
        adjList[from].push_back(to);
    }

    return adjList;
}

int inDegree(int node, const std::unordered_map<int, std::vector<int>>& adjList)
{
    int inDeg = 0;
    for(int i = 1; i <= n; ++i) 
    {
        if(i == node) continue;       
        if(adjList.find(i) != adjList.end()) 
        {
            for(int neighbor : adjList.at(i)) 
            {
                if(neighbor == node) ++inDeg;
            }
        }
    }

    return inDeg;
}

int outDegree(int node, const std::unordered_map<int, std::vector<int>>& adjList)
{
    if(adjList.find(node) == adjList.end()) return 0;
    return adjList.at(node).size();
}

//Basically, the judge is the node with inDegree = n-1 and outDegree = 0
int findJudge(int n, std::vector<std::vector<int>>& trust) 
{
    auto adjList = build_adjList(trust);
}