#pragma once

//leetcode.com/problems/student-attendance-record-ii/description/
int checkRecord(int n) 
{
 /*
	The method used here is called 'matrix exponentiation'.
	
	This a neat technique to compute terms of a linear recurrence (or apply a fixed linear transform repeatedly) in O(logn) time, 
	rather than O(n). 
	
	It’s especially useful when you have a fixed-size state vector and a constant transition matrix M, and you want M^n
	
	First, express the problem as a state transition matrix,

	a ∈ {0,1} → how many 'A's used so far (0 or 1).
	l ∈ {0,1,2} → how many consecutive trailing 'L's.

	That gives 6 states:
	(a,l) --> {(0,0),(0,1),(0,2),(1,0),(1,1),(1,2)}

	From any state (a, l):
		Add 'P' → (a, 0)   (resets L streak)
		Add 'L' → (a, l+1) only if l < 2
		Add 'A' → (1, 0)   only if a == 0
		
	6 states, so a 6x6 state transition matrix,
	Initialize all M[i][j] = 0

	# From 0:(0,0)
	M[0][0] += 1    # 'P' -> (0,0)
	M[1][0] += 1    # 'L' -> (0,1)
	M[3][0] += 1    # 'A' -> (1,0)

	# From 1:(0,1)
	M[0][1] += 1    # 'P' -> (0,0)
	M[2][1] += 1    # 'L' -> (0,2)
	M[3][1] += 1    # 'A' -> (1,0)

	# From 2:(0,2)
	M[0][2] += 1    # 'P' -> (0,0)
	M[3][2] += 1    # 'A' -> (1,0)
	# no 'L' because l=2 cannot go to 3 L's

	# From 3:(1,0)
	M[3][3] += 1    # 'P' -> (1,0)
	M[4][3] += 1    # 'L' -> (1,1)
	# no 'A' because already used one A

	# From 4:(1,1)
	M[3][4] += 1    # 'P' -> (1,0)
	M[5][4] += 1    # 'L' -> (1,2)

	# From 5:(1,2)
	M[3][5] += 1    # 'P' -> (1,0)
	# no 'L' and no 'A'
	
	
	Now, goal after 'n' steps is a valid string of size n. This is equivalent to computing:
	v_n= M^n . v_0

	v_0 is the initial state vector: [1, 0, 0, 0, 0, 0] (only state 0 is active at the start).
	M^n is the transition matrix raised to the power n, representing n steps of transitions.
	v_n is the resulting vector: it tells you how many valid strings of length n end in each of the 6 states.

 */
 
 /*
	Pseudo code for the operations involved... note since numbers can become huge, question specifies modulo 
	
	1. Matrix Multiply
	
	function matMul(A, B):
		C = zero 6x6 matrix
		for i in 0..5:
			for k in 0..5:
				if A[i][k] == 0: continue
				for j in 0..5:
					C[i][j] = (C[i][j] + A[i][k] * B[k][j]) mod MOD
		return C

	2. Matrix and Vector multiply
		
	function matVecMul(M, v):
		u = zero vector length 6
		for i in 0..5:
			sum = 0
			for j in 0..5:
				sum = (sum + M[i][j] * v[j]) mod MOD
			u[i] = sum
		return u

	2. Fast Power matPow(M, n) using binary exponentiation
	
	Binary exponentation is basically, jumping to required 'n' in powers of 2,
    M^n = M^(n/2) . M^(n/2) ... if n is even
	M^n = M . M^(n-1) ... if n is odd 
	
	function matPow(base, n):
		# result = identity matrix
		result = identity 6x6
		while n > 0:
			if (n % 2 == 1):
				result = matMul(result, base)
			base = matMul(base, base)
			n = n // 2
		return result
 */
 
 /*
 finally, 
 
 
function checkRecord(n):
    # 1) Build M (6x6) using transitions above
    M = zero 6x6
    M[0][0] += 1; M[1][0] += 1; M[3][0] += 1
    M[0][1] += 1; M[2][1] += 1; M[3][1] += 1
    M[0][2] += 1; M[3][2] += 1
    M[3][3] += 1; M[4][3] += 1
    M[3][4] += 1; M[5][4] += 1
    M[3][5] += 1

    # 2) v0
    v0 = [1, 0, 0, 0, 0, 0]

    # 3) Compute M^n
    Mn = matPow(M, n)

    # 4) vn = Mn * v0
    vn = matVecMul(Mn, v0)

    # 5) Sum all states
    ans = 0
    for i in 0..5:
        ans = (ans + vn[i]) mod MOD
    return ans


 */
}//leetcode.com/problems/student-attendance-record-ii/description/
int checkRecord(int n) 
{
 /*
	The method used here is called 'matrix exponentiation'.
	
	This a neat technique to compute terms of a linear recurrence (or apply a fixed linear transform repeatedly) in O(logn) time, 
	rather than O(n). 
	
	It’s especially useful when you have a fixed-size state vector and a constant transition matrix M, and you want M^n
	
	First, express the problem as a state transition matrix,

	a ∈ {0,1} → how many 'A's used so far (0 or 1).
	l ∈ {0,1,2} → how many consecutive trailing 'L's.

	That gives 6 states:
	(a,l) --> {(0,0),(0,1),(0,2),(1,0),(1,1),(1,2)}

	From any state (a, l):
		Add 'P' → (a, 0)   (resets L streak)
		Add 'L' → (a, l+1) only if l < 2
		Add 'A' → (1, 0)   only if a == 0
		
	6 states, so a 6x6 state transition matrix,
	Initialize all M[i][j] = 0

	# From 0:(0,0)
	M[0][0] += 1    # 'P' -> (0,0)
	M[1][0] += 1    # 'L' -> (0,1)
	M[3][0] += 1    # 'A' -> (1,0)

	# From 1:(0,1)
	M[0][1] += 1    # 'P' -> (0,0)
	M[2][1] += 1    # 'L' -> (0,2)
	M[3][1] += 1    # 'A' -> (1,0)

	# From 2:(0,2)
	M[0][2] += 1    # 'P' -> (0,0)
	M[3][2] += 1    # 'A' -> (1,0)
	# no 'L' because l=2 cannot go to 3 L's

	# From 3:(1,0)
	M[3][3] += 1    # 'P' -> (1,0)
	M[4][3] += 1    # 'L' -> (1,1)
	# no 'A' because already used one A

	# From 4:(1,1)
	M[3][4] += 1    # 'P' -> (1,0)
	M[5][4] += 1    # 'L' -> (1,2)

	# From 5:(1,2)
	M[3][5] += 1    # 'P' -> (1,0)
	# no 'L' and no 'A'
	
	
	Now, goal after 'n' steps is a valid string of size n. This is equivalent to computing:
	v_n= M^n . v_0

	v_0 is the initial state vector: [1, 0, 0, 0, 0, 0] (only state 0 is active at the start).
	M^n is the transition matrix raised to the power n, representing n steps of transitions.
	v_n is the resulting vector: it tells you how many valid strings of length n end in each of the 6 states.

 */
 
 /*
	Pseudo code for the operations involved... note since numbers can become huge, question specifies modulo 
	
	1. Matrix Multiply
	
	function matMul(A, B):
		C = zero 6x6 matrix
		for i in 0..5:
			for k in 0..5:
				if A[i][k] == 0: continue
				for j in 0..5:
					C[i][j] = (C[i][j] + A[i][k] * B[k][j]) mod MOD
		return C

	2. Matrix and Vector multiply
		
	function matVecMul(M, v):
		u = zero vector length 6
		for i in 0..5:
			sum = 0
			for j in 0..5:
				sum = (sum + M[i][j] * v[j]) mod MOD
			u[i] = sum
		return u

	2. Fast Power matPow(M, n) using binary exponentiation
	
	Binary exponentation is basically, jumping to required 'n' in powers of 2,
    M^n = M^(n/2) . M^(n/2) ... if n is even
	M^n = M . M^(n-1) ... if n is odd 
	
	function matPow(base, n):
		# result = identity matrix
		result = identity 6x6
		while n > 0:
			if (n % 2 == 1):
				result = matMul(result, base)
			base = matMul(base, base)
			n = n // 2
		return result
 */
 
 /*
 finally, 
 
 
function checkRecord(n):
    # 1) Build M (6x6) using transitions above
    M = zero 6x6
    M[0][0] += 1; M[1][0] += 1; M[3][0] += 1
    M[0][1] += 1; M[2][1] += 1; M[3][1] += 1
    M[0][2] += 1; M[3][2] += 1
    M[3][3] += 1; M[4][3] += 1
    M[3][4] += 1; M[5][4] += 1
    M[3][5] += 1

    # 2) v0
    v0 = [1, 0, 0, 0, 0, 0]

    # 3) Compute M^n
    Mn = matPow(M, n)

    # 4) vn = Mn * v0
    vn = matVecMul(Mn, v0)

    # 5) Sum all states
    ans = 0
    for i in 0..5:
        ans = (ans + vn[i]) mod MOD
    return ans


 */
}