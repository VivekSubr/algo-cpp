#pragma once
#include <vector>
#include <algorithm>

//Refer: https://howardhinnant.github.io/combinations.html

//C++ has std::next_permutation, https://en.cppreference.com/w/cpp/algorithm/next_permutation
//

template <typename T>
class Combination
{
public:
    Combination(const std::vector<T>& elements): m_elements(elements) 
    {
    }    

    std::vector<std::vector<T>> getCombinations(int K)
    {
        std::vector<std::vector<T>> ret;

        std::vector<char> bitmask(m_elements.size(), 0);
        for(int i=0; i<K; i++) { bitmask[i] = 1; } // K leading 1's, N-K trailing 0's

        do 
        {
            std::vector<T> single_permutation;
            for (size_t i = 0; i < m_elements.size(); i++) // [0..N-1] integers
            {
                if(bitmask[i]) 
                {
                    single_permutation.push_back(m_elements[i]);
                }
            }

            ret.emplace_back(std::move(single_permutation));
            
        //Using prev_permutation, as bitmask has leading 1s, which is actually the last lexiographic ordering.
        //Hence need to go back lexiographically
        } while(std::prev_permutation(bitmask.begin(), bitmask.end()));
    
        return ret;
    }

    std::vector<std::vector<T>> getPermutations(const std::vector<T>& arr)
    {
        std::vector<std::vector<T>> ret;
      
        do 
        {
            ret.push_back(arr);
        } while(std::next_permutation(arr.begin(), arr.end()));
      
        return ret;
    }

private:
    std::vector<T> m_elements;
};