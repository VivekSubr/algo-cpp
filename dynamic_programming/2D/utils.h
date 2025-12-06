#pragma once
#include <vector>
#include <iostream>

template<typename T>
void print_matrix(const std::vector<std::vector<T>>& M)
{
    for(const auto& row : M)
    {
        for(const auto& val : row)
        {
            std::cout<<val<<" ";
        }
        std::cout<<"\n";
    }
}