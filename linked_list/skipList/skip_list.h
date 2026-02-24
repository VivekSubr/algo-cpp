#pragma once
#include <vector>
#include <memory>

template <typename T>
class skip_list 
{
public:
    struct node
    {
        T value;
        std::vector<std::shared_ptr<node>> links; 
    };

    skip_list(size_t max_level) : m_max_level(max_level)
    {
    }

    void insert(const T& value) 
    {
    }

    void del(const T& value) 
    {
    }

    std::shared_ptr<node> find(const T& value) 
    {
        return nullptr;
    }

private:
    std::shared_ptr<node> m_head;
    size_t                m_max_level;
};