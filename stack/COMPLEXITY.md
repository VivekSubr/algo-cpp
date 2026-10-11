# Stacks 
Seems simple, but have to think it though. Key thing is the FILO (First In, Last Out) nature.

Consider the simplest piece of code, the paranthesis check ->
```
bool check_if_valid(const std::string& result)
{
    std::stack<char> s;
    for(auto ch : result) 
    {
        if(ch == '(') s.push(ch);
        else if(ch == ')')
        {
            if(s.empty()) return false;
            s.pop();
        } 
    }

    return s.empty();
}
```

This works since the brackets can't be mixed - '((()))' is valid, but '(()()' is not... hence every push and pop maust match up, which is what we are checking.

Conside the multiple type of brackets version as well,
```
bool isValid(string s) 
{
    std::stack<char> ts;
    for (auto ch : s) 
    {
        if (ch == '(' || ch == '{' || ch == '[')
        {
            ts.push(ch);
        }
        else
        {
            if (ts.empty()) return false;
            char top = ts.top();
            if ((ch == ')' && top != '(') ||
                (ch == '}' && top != '{') ||
                (ch == ']' && top != '['))
            {
                return false;
            }
            ts.pop();
        }
    }
    return ts.empty();
}
```

One additional check - the next bracket (at .top()) *must* be of the correct type as well.


## Monotonic Stack
Now, let's get to how stacks come in the picture, congnitively, for problems like **Daily Temperatures**

"You are given an array of integers temperatures where temperatures[i] represents the daily temperatures on the ith day.

Return an array result where result[i] is the number of days after the ith day before a warmer temperature appears on a future day. If there is no day in the future where a warmer temperature will appear for the ith day, set result[i] to 0 instead."

One way to solve can be *two pointer*, with the second pointer finding the number of days for every index... this will be O(n squared).

Now, the pattern here is *monotonic stack*... the **the stack is a list of "still waiting for an answer" indices**.

**Important**
At it's core, the algo is basically,
```
    stack = []                      # store indices, not values, when you need position info
    for i in range(n):
        while stack and breaks_invariant(nums[stack[-1]], nums[i]):
            j = stack.pop()
            answer[j] = i           # or nums[i], or i - j, depending on what's asked
        stack.append(i)
    # whatever's left in the stack never found a match — default them (-1, 0, len, etc.)
```

So... basically, we have 
1. stack of days (indices)
```
    std::stack<size_t> s
    for(std::size_t i = 0; i < temperatures.size(); i++)
```

Now, the invariant... "Warmer day than today", ie,
    ```while(!s.empty() && temp.at(i) > temp.at(s.top()))```

Idea - s.top() is today, ie a day waiting for an answer... pop() when you've found a day warmer than it.

```
    while(!s.empty() && temp.at(i) > temp.at(s.top()))
    {
        ret.at(s.top()) = i - s.top();
        s.pop();
    }
```
    