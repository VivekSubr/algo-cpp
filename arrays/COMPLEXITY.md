Here's we will go though complexity of the problems

## Anagrams 

```
bool are_anagram(const std::string& s, const std::string& t)
{
    if(t.size() != s.size()) return false;
    std::unordered_set<char> s_set, t_set;
    for(auto sch : s)  { s_set.insert(sch); }
    for(auto tch : t)  { t_set.insert(tch); }

    return s_set.size() == t_set.size();
}
```

Set insertions are O(1)... hence, O(size of s) + O(size of t), consider O(n)

## Matrix 

**Spiral Matrix**
Visits every element once, hence O(row + col)... O(n)

**Maximal Square**
The nested for execute fully, nothing to break it... hence O(row * col), O(n squared)

**Longest Increasing Subsequence**
Again, nested for executes fully... O(n squared)

## Prefix Sum
Problem statement is  - sumRange(int left, int right), should return sum between left and right indices.

Beautiful example of pre-computation here, the constructor pre-completes prefix for all elements, an O(n) operation, so that the query itself is O(1)

## Text Justify
Now, this is a complex example for complexity analysis.

First step, trace out the flow, while --> if/else... main question is -> how many times will the while execute?