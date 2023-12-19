#pragma once
#include <vector>
#include <algorithm>

template <typename T>
class Combination
{
public:


private:
    std::vector<T> m_elements;

    std::vector<std::vector<T>> getCombinations(const std::vector<T>& elements, int K)
    {
        std::vector<std::vector<T>> ret;

        std::vector<char> bitmask(elements.size(), 0);
        for(int i=0; i<N; i++) { bitmask[i] = 1; } // K leading 1's, N-K trailing 0's

        do 
        {
            for (int i = 0; i < N; ++i) // [0..N-1] integers
            {
                if (bitmask[i]) std::cout << " " << i;
            }
            std::cout << std::endl;
            
        } while(std::prev_permutation(bitmask.begin(), bitmask.end()));
    }
};