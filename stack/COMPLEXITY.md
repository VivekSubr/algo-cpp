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

Instead of thinking of the stack as holding temperatures, think of it as holding unresolved days — days for which you haven't yet found a warmer future day. You scan left to right. 

For each new day: While the new temperature is warmer than the temperature at the index on top of the stack, that top index just got its answer: pop it, and its result is ```current_index - popped_index.```

Once the new temperature is no longer warmer than the top (or the stack is empty), push the current index. It's now a new "unresolved" day, waiting.