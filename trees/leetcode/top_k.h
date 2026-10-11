#pragma once
#include <vector>
#include <queue>
#include <unordered_map>

class TopK
{
public:
    TopK(std::vector<int>&& arr): m_data(arr) 
    {
	    m_max_pq = std::priority_queue<int>(m_data.begin(), m_data.end());
        for(auto ele : m_data) { m_freq[ele]++; }
    }
 
    void add(int val)
    {
	    m_data.push_back(val);
	    m_max_pq.push(val);
        m_freq[val]++;
        auto it = m_by_freq.find(val);
        m_by_freq[it->second] = it->first; 
    }

    int k_largest(int k)
    {
	    int top;
	    std::vector<int> temp_k;
	    while(k-- > 0)
	    {
	        top = m_max_pq.top();
	        m_max_pq.pop();
	        temp_k.push_back(top);
	    }

	    for(auto ele : temp_k) { m_max_pq.push(ele); }
	    return top;
    }

    int k_frequent(int k)
    {
        if(k > m_by_freq.size()) return -1;

        auto k_ele = m_by_freq.begin();
        std::advance(k_ele, k - 1); //m_by_freq.begin() + k only works with random access iterators
        
        if(k_ele == m_by_freq.end()) return -1;
        return k_ele->second;
    }

private:
    std::vector<int>             m_data;
    std::priority_queue<int>     m_max_pq;     
    std::map<int, int>           m_by_freq;
    std::unordered_map<int, int> m_freq;
};