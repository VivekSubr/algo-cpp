#pragma once

<typename T>
class bloom_filter 
{
public:
    bloom_filter() 
    {
    }

    void add(const T& ele)
    {
    }

    void remove(const T& ele)
    {
    }

    bool exists(const T& ele)
    {
        return false;
    }

private:
    std::vector<T> m_data;
};