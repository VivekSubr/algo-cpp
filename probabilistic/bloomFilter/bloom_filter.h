#pragma once
#include <vector>
#include <functional>

template<typename T>
class bloom_filter 
{
public:
    using BloomHasher = std::function<size_t(const T&)>;

    bloom_filter(BloomHasher&& hasher, const size_t bitSz=64): m_bitmap(bitSz, false), m_hasher(std::move(hasher))
    {
    }

    void add(const T& ele)
    {
        m_bitmap[m_hasher(ele) % m_bitmap.size()] = true;
    }

    void remove(const T& ele)
    {
        m_bitmap[m_hasher(ele) % m_bitmap.size()] = false;
    }

    bool exists(const T& ele)
    {
        return m_bitmap[m_hasher(ele) % m_bitmap.size()];
    }

private:
    std::vector<bool> m_bitmap;
    BloomHasher       m_hasher;
};