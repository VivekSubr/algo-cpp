# Dynamic Programming
Two things for this - 
* Frame the problem as a **series of decisions**, like say as tree or grid structure, and then compute the solution... DP problems **must** be expressible in some form as ```f(x2) = f(x1)```

* Intermediate results should be stored so that they aren't re-computed... this is called **memoization** 

## DP is Backtracing + Caching
Backtracking is the process of searching though the tree or grid of possibilities and discarding paths that don't work.

# Compexity
Usually,
* Solve the work function (the one thats recurisive)
* Multiply by number of distinct states.

That's why best to keep work function close to O(1).

**Master Theorem**
The master theorem is a shortcut for solving recurrences like T(n) = a·T(n/b) + f(n), so you don't have to draw a recursion tree each time.

"Split problem of size 'n' into 'a' subproblems of n/b complexity, and finally add work done after recursion is done as f(n)"

Eg - Binary Search... we are splitting the array in half, hence T(n/2), and recursion is linear (no child branches)... hence, T(n) = T(n/2) + 1. Not much work done after result is found, but f(n) is never 0, even the return is abit of work.

In terms of Complexity... O(logn). 

One more example - Merge Sort.
1. Divide into two sub-arrays, continue till they can be trivialy recombined (classic example divides till single elements)
2. Recombine in sorted order.

The array is halved, hence sub-problem is T(n/2)... each problem breaks into two sub problems... hence a=2. Recombine is trivial, hence f(n) = 1.

T(n) = 2.T(n/2) + 1

Complexity... O(nlogn)

## Word Search
Master theorem can not be applied to this one, because
1. The search is not shrinking the problem space
2. Memoization breaks the recurrence. 

But, let's apply anyway, so roughly -> T(n) = 4.T(n) + 1

Hence, O(4^n) complexity, without memoization.

## Jump Game

## Max Sub-Array