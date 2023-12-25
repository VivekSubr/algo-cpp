#pragma once
#include "graph.h"

//https://www.reddit.com/r/algorithms/comments/10r2pto/can_someone_explain_union_find_but_do_it_using/
//https://www.youtube.com/watch?v=8f1XPm4WOUc


class DisjointSet 
{
public:
    DisjointSet(std::shared_ptr<Graph> graph): m_graph(graph), m_parentArr(graph->getNumberNodes(), -1), m_rankArr(graph->getNumberNodes(), 0)
    {
        for(int i=0; i<graph->getNumberNodes(); i++)
        {
            m_parentArr[i] = i; //Set parent to itself to begin with
        }
    }

    //count the number of 'connected components' in a graph, ie, the number of connected sub-graphs with no edges between them
    int count_connected_components()
    {
        int nCount = m_graph->getNumberNodes(); //worst case, every node is un-connected
        
        for(auto& edge : m_graph->getEdges())
        {
            if(do_union(std::get<0>(edge), std::get<1>(edge)))
            {
                --nCount;
            }
        }

        return nCount;
    }

private:
    std::shared_ptr<Graph> m_graph; //pointer to the under-lying graph

    std::vector<int> m_parentArr; //index - value represents one edge
    std::vector<int> m_rankArr;   //'rank' of a node is the size of the connected component it belongs to

    //returns root of node's disjoint set
    int find(int node)
    {
        int ret = node; //If completely disjoint, node is own parent
        while(ret != m_parentArr[ret])
        {
            ret = m_parentArr[ret]; 
        }

        return ret;
    }

    bool do_union(int n1, int n2)
    {
        //Find the parents
        int p1 = find(n1);
        int p2 = find(n2);

        if(p1 == p2) return false; //already in same connected component

        if(m_rankArr[p2] > m_rankArr[p1]) //prefer to union with the larger component
        {
            m_parentArr[p2] = p1;
            m_rankArr[p2]++;
        }
        else
        {
            m_parentArr[p1] = p2;
            m_rankArr[p1]++;
        }

        return true;
    } 
};

