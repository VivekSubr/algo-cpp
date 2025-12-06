#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>    

//leetcode.com/problems/satisfiability-of-equality-equations/description


/*
	We have up to 26 variables: a … z.
	Equations are either:
		Equality: x==y → x and y must be in the same set.
		Inequality: x!=y → x and y must be in different sets.   
		
	Hence these form 'disjoin sets'... so, we do 
		First pass: Union all pairs from equality equations (==).
		Second pass: Check all inequalities (!=).

		If any inequality has the same representative (root) for both variables, it’s contradiction → return false.
		If no contradictions → return true.
*/  



struct DSU 
{
    std::unordered_map<char, char> parent; // var -> parent (root points to itself)
    std::unordered_map<char, int>  sz;     // var -> component size

    char find(char x) 
    {
		if(parent.find(x) == parent.end()
		{
			parent[x] = x;
            sz[x] = 1;
		}
		
        if(parent.at(x) == x) return x;
		
        parent.at(x) = find(parent[x]); // path compression
		return parent.at(x);
    }

    void do_union(char a, char b) 
	{
        a = find(a);
        b = find(b);
        if (a == b) return;
        if (sz[a] < sz[b])  parent[a] = b;
        else                parent[b] = a;

        sz[a] += sz[b];
    }
};


   
bool equationsPossible(std::vector<std::string>& equations) 
{
	
	DSU dsu;

    // 1) Union all equalities
    for (const string& eq : equations) {
        if (eq[1] == '=' && eq[2] == '=') {
            dsu.do_union(eq[0], eq[3]);
        }
    }

    // 2) Check all inequalities
    for (const string& eq : equations) {
        if (eq[1] == '!' && eq[2] == '=') {
            if (dsu.find(eq[0]) == dsu.find(eq[3])) {
                // same set but required to be different → contradiction
                return false;
            }
        }
    }

    return true;

}