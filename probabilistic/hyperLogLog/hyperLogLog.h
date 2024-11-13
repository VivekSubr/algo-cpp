#pragma once
#include <xxhash.h>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>
#include <limits>
#include <iomanip>
#include <iostream>
#include <cstring>
#include <random>

/*

What is hyperLogLog? 
--> It is an algo to estimate cardinality of a dataset, ie number of distinct elements.
    Note, it *estimates*, not counts.

Let's go through the thinking process that led to HyperLogLog. 

(Redis implementation: https://github.com/redis/redis/blob/unstable/src/hyperloglog.c)

*/

/*

* Simple estimator
Given n numbers, with duplicates and distributed evenly, then hash the values.
Now, assuming that the numbers are perfectly random and uniformly distributed (properties provided by the hash), then
cardinatly will be approximatey max/min.

Make the hash range [0, 1], then 1/min will be approx cardinality.

*/

double hashNormalize(int val)
{
    double uint32_max = std::pow(2, 32);
    return XXH32(&val, sizeof(val), 0)/uint32_max;
}

uint simple_estimator(const std::vector<uint>& arr)
{
    std::vector<double> hash_arr;
    std::for_each(arr.begin(), arr.end(), [&](uint val) { hash_arr.push_back(hashNormalize(val)); }); 

    double min = *std::min_element(hash_arr.begin(), hash_arr.end());
    return static_cast<uint>(1.0/min);
}

/*

* Probabilistic estimator
We rely on an observation - for perfectly random data, a sequence of k bits will occur only once for 2^k elements. 
So, we hash the values (and rely on the hashing algo resulting in a perfectly random and uniform distribution), and count
the max number of leading 0s seen in the dataset. That is the cardinality.

*/

enum class probabalistic_estimate_type 
{
    leading_zeroes  = 0,
    trailing_zeroes 
};

uint count_leading_zeros(double val)
{
    //warning, we're losing the fractional values here, this will truncate.
    uint64_t binaryRepresentation = static_cast<uint64_t>(val); 

    uint leadingZeroes = 0;
    for (int i = 63; i >= 0; --i) 
    {
        //1ULL is 1 as unsigned long long.
        //Hence first step is right shift 1 63 bits and mask, and then so on.
        if((binaryRepresentation & (1ULL << i)) == 0) ++leadingZeroes;
        else                                          break;
    }

    return leadingZeroes;
}

uint count_trailing_zeros(double val)
{
    uint64_t binaryRepresentation = static_cast<uint64_t>(val);

    uint trailingZeroes = 0;
    for (int i = 0; i < 64; i++) 
    {
        if((binaryRepresentation & (1ULL << i)) == 0) ++trailingZeroes;
        else                                          break;
    }

    return trailingZeroes;
}

uint probabilistic_estimator(const std::vector<uint>& arr, probabalistic_estimate_type t)
{
    uint max_zeroes;
    std::for_each(arr.begin(), arr.end(), [&](uint val) 
    { 
        uint64_t hashVal = XXH64(&val, sizeof(val), 0); 
        uint zeroes      = 0;
        switch(t)
        {
            case probabalistic_estimate_type::leading_zeroes:
                zeroes = count_leading_zeros(hashVal); break;

            case probabalistic_estimate_type::trailing_zeroes: 
                zeroes = count_trailing_zeros(hashVal); break;
        }

        if(zeroes > max_zeroes) max_zeroes = zeroes;
    }); 

    return max_zeroes;
}

/*

* LogLog
As can be seen in the gtest, probabilistic estimator gives bad results when data set properties aren't met (or dataset is small).
One way is to increase and improve the dataset by using n hash functions instead of one, and averaging the output... this is 
however expensive, so another way is to use a single hash function but break it's output to 'm' buckets. 

For m buckets, we use first k bits of hash value as index to mth bucket.
eg: hashValue is 1011011101101100000, k = 4 then 1011 = 11, therefore update 11th bucket with trailing/leading zeroes of remaining, 011101101100000.

One we get estimate of each bucket, {R0, R1.... Rm}, then cardinality is estimated as,

C = m*2^((R0 + R1 + ... Rm))/m). 

It was latter found that this formula has a bias, so a bias factor of value 0.79402 was multiplied with it.

Standard error of this method was found to be 3/sq_root(m)

*/

uint bucket_index(uint64_t val, uint k)
{
    //create mask of first k bits.
    uint64_t mask = 0;
    for(uint i=1; i<=k; i++) {
        mask |= (1ULL << (64 - i));
    }
    
    uint64_t index = val & mask;
    //std::cout<<"bucket_index "<<index<<" and "<<binaryVal<<" and "<<mask<<"\n";

    //now take the first k bits and place them in last position
    uint64_t normalizedIndex = 0;
    for(uint i=1; i<=k; i++) 
    {
        if(index & (1ULL << (64 - i))) normalizedIndex |= (1ULL << (k - i));
    }

    return normalizedIndex;
}

uint count_leading_zeros(uint64_t val, uint k)
{
    uint leadingZeroes = 0;
    for(int i = 63; i >= 0; --i) 
    {
        //1ULL is 1 as unsigned long long.
        //Hence first step is right shift 1 63 bits and mask, and then so on.
        if((val & (1ULL << i)) == 0) ++leadingZeroes;
        else                         break;
    }

    return leadingZeroes - k;
}

uint count_trailing_zeros(uint64_t val, [[maybe_unused]] uint k)
{
    uint trailingZeroes = 0;
    for (int i = 0; i < 64; i++) 
    {
        if((val & (1ULL << i)) == 0) ++trailingZeroes;
        else                         break;
    }

    return trailingZeroes;
}

uint loglog(const std::vector<uint64_t>& arr, uint k, probabalistic_estimate_type t)
{
    std::unordered_map<uint, uint> buckets;
    std::for_each(arr.begin(), arr.end(), [&](uint val) 
    {
        uint64_t hashVal = XXH64(&val, sizeof(val), 0); 
        uint index     = bucket_index(hashVal, k);
        uint zeroes    = 0;
        switch(t)
        {
            case probabalistic_estimate_type::leading_zeroes:
                zeroes = count_leading_zeros(hashVal, k); break; 

            case probabalistic_estimate_type::trailing_zeroes: 
                zeroes = count_trailing_zeros(hashVal, k); break;
        }

        if(buckets.find(index) == buckets.end())
        {
            buckets.insert({index, zeroes});
        }
        else if(buckets.at(index) < zeroes) buckets.at(index) = zeroes; 
    }); 

    uint m = buckets.size();
    uint sum = 0;
    for(auto bucket : buckets) { sum += bucket.second; }

    std::cout<<"loglog bucket size "<<m<<" sum "<<sum<<"\n";
    return 0.79402*m*std::pow(2, sum/m);
}

/*

HyperLogLog is a refinement of loglog.

m -> number of registers/buckets used. Has to satisfy m = 2^p, where p is the number of bits used for indexing the register.
Aggregation into the registers works as follows,
for all values in Set do:
    x = hash(val)
    index  = first p bits of x
    remain = value of x with first p bits removed
    them register[index] = max(register[index], number of leading zeroes in (remain + 1)) [!]

Then, Estimate = (bias constant) * m * m * 1/(Sum 0 to m-1 of 2^-M[j]).

Note, for small cardinalities, estimates can have large errors, and we generally fall back to linear counting.

*/

std::pair<uint64_t, uint64_t> index_and_remainder(uint64_t val, uint p)
{
    //create mask of first p bits.
    uint64_t index_mask = 0;
    for(uint i=1; i<=p; i++) {
        index_mask |= (1ULL << (64 - i));
    }

    //create mask of remaining bits 
    uint64_t remain_mask = 0;
    for(uint i=p+1; i<=64; i++) {
        remain_mask |= (1ULL << (64 - i));
    }

    uint64_t index = val & index_mask;

    //now take the first p bits and place them in last position
    uint64_t normalizedIndex = 0;
    for(uint i=1; i<=p; i++) 
    {
        if(index & (1ULL << (64 - i))) normalizedIndex |= (1ULL << (p - i));
    }
    
    uint64_t remain = val & remain_mask;
    return std::make_pair(normalizedIndex, remain);
}

uint count_zeroes(uint64_t remainder, uint p)
{
    //number of leading zeroes in remainder + 1.
    remainder = remainder + 1;
    uint zeroes = 0;
    for (uint i = 63; i >= p; --i) 
    {
        if(!(remainder & (1ULL << i))) ++zeroes;
        else                           break;
    }

    return zeroes;
}

uint hyperloglog(const std::vector<uint64_t>& arr, uint p)
{
    uint m = std::pow(2, p);
    std::vector<uint> buckets(m, 0);
    for(uint val : arr)
    {
        uint64_t hashVal = XXH64(&val, sizeof(val), 0); 
        auto in_remain = index_and_remainder(hashVal, p);
        uint zeroes    = count_zeroes(in_remain.second, p);

        if(buckets[in_remain.first] < zeroes) {
            buckets[in_remain.first] = zeroes;
        }
    }

    double sum = 0;
    for(uint i=0; i<m; i++)
    {
        int exp = -1*buckets[i]; //https://stackoverflow.com/questions/53193707/c-function-stdpow-returns-inf-instead-of-a-value
        sum += std::pow(2, exp); 
    }

    std::cout<<"buckets "<<buckets.size()<<" sum "<<sum<<"\n";
    return  0.79402*m*m/sum;
}

/*

A further refinement of this algorithm.
https://static.googleusercontent.com/media/research.google.com/en//pubs/archive/40671.pdf

* HLL has large errors for small cardinality, so HLL++ passes the resulting estimate through a Bias Correction Step.
* Optionally, HLL++ allows compressing bits in the register indices to make do in less size.

Otherwise, same as hyperloglog.

*/

std::vector<uint> getNearestNeighbors(uint estimate, const std::vector<uint>& comparision)
{
    std::vector<std::pair<uint, int>> distances;

    for(size_t i=0; i < comparision.size(); ++i)
        distances.emplace_back(std::pow((estimate - comparision[i]), 2), i);

    std::vector<uint> neighbors;
    static const size_t kDistanceThreshold = 6; //google's hyperloglog paper chooses 6, and says it's good enough for all cases.
    for (size_t i=0; i < std::min(kDistanceThreshold, distances.size()); ++i)
        neighbors.push_back(distances[i].second); // Add index of close neighbor

    return neighbors;
} 

//estimate bias using KNN algorithm
uint estimate_bias(uint estimate)
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, UINT64_MAX);
    
    std::vector<uint> comparision;
    for(int i=0; i<200; i++) { //roughly 200 cardinality
        comparision.push_back(dist(rng));
    }
    
    auto nn  = getNearestNeighbors(estimate, comparision);
    uint sum = 0;
    for(auto neighbor : nn) {
        sum += neighbor;
    }

    return sum/nn.size();   
}

uint hyperloglog_plusplus(const std::vector<uint64_t>& arr, uint p)
{
    uint m = std::pow(2, p);
    double alpha = 0.709;
    switch(m)
    {
        case 16: alpha = 0.673; break;
        case 32: alpha = 0.697; break;
        case 64: alpha = 0.709; break;
        default:
            if(m >= 128) alpha = 0.7213/(1 + 1.079/m);
    } 

    std::vector<uint> buckets(m, 0);
    for(uint val : arr)
    {
        uint64_t hashVal = XXH64(&val, sizeof(val), 0); 
        auto in_remain = index_and_remainder(hashVal, p);
        uint zeroes    = count_zeroes(in_remain.second, p);

        if(buckets[in_remain.first] < zeroes) {
            buckets[in_remain.first] = zeroes;
        }
    }

    double sum = 0;
    for(uint i=0; i<m; i++)
    {
        int exp = -1*buckets[i]; //https://stackoverflow.com/questions/53193707/c-function-stdpow-returns-inf-instead-of-a-value
        sum += std::pow(2, exp); 
    }

    uint estimate = alpha*m*m/sum;
    uint bias_correction = estimate_bias(estimate);
    std::cout<<"sum "<<sum<<" estimate "<<estimate<<" bias factor "<<bias_correction<<"\n";

    if(estimate <= 5*m) return estimate - bias_correction;
    else                return estimate;
}