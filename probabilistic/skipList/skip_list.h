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

/*
When search for k:
    If k = key, done!
    If k < next key, go down a level
    If k ≥ next key, go righ
*/

/*
Find k
Insert node in level 0
let i = 1
while FLIP() == “heads”:
    insert node into level i
    i++
*/

private:
    std::shared_ptr<node> head;
};