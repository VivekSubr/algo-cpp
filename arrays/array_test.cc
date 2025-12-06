#include "array_test.h"
#include "text_justify.h"
#include "prefix_sum.h"
#include "matrix.h"
#include <tuple>
#include <numeric>

TEST_F(ArrayTest, TestJustify)
{
    std::vector<std::tuple<std::vector<std::string>, int>> tests =
    {
        { {"This", "is", "an", "example", "of", "text", "justification."}, 16 }
    };

    for(auto test : tests)
    {
      auto ret = fullJustify(std::get<0>(test), std::get<1>(test));
      std::cout<<"got ret "<<ret.size()<<"\n";
      printVector(ret);
    }
}

TEST_F(ArrayTest, RangeSumQueryImmutable)
{
  std::vector<std::tuple<std::vector<int>, std::vector<std::pair<int,int>>, std::vector<int>>> tests =
  {
    { { -2, 0, 3, -5, 2, -1 }, { {0,2}, {2,5}, {0,5} }, { 1, -1, -3} }
  };

  for(auto test : tests)
  {
    NumArray na(std::get<0>(test));
    auto queries  = std::get<1>(test);
    auto expected = std::get<2>(test);  
    for(size_t i = 0; i < queries.size(); ++i)
    {
      auto p = queries[i];
      auto ret = na.sumRange(p.first, p.second);
      ASSERT_EQ(ret, expected[i]);
    }
  }
}

TEST_F(ArrayTest, LongestIncreasingPath)
{
  std::vector<std::tuple<std::vector<std::vector<int>>, int>> tests = {
    { { {9,9,4}, {6,6,8}, {2,1,1} }, 4 },
    { { {3,4,5}, {3,2,6}, {2,2,1} }, 4 },
    { { {1} }, 1 }
  };

  for(auto test : tests)
  {
    auto ret = longestIncreasingPath(std::get<0>(test));
    ASSERT_EQ(ret, std::get<1>(test));
  }
}

TEST_F(ArrayTest, SubarraySum)
{
    std::vector<std::tuple<std::vector<int>, int, int>> tests = {
        {{1,1,1}, 2, 2},
        {{1,2,3}, 3, 2}
    };

    for(auto ele : tests)
    {
        ASSERT_EQ(subarraySum(std::get<0>(ele), std::get<1>(ele)), std::get<2>(ele))
            << "Failed for input " << 
            std::accumulate(std::get<0>(ele).begin(), std::get<0>(ele).end(), std::string(""), 
            [](const std::string& a, int b) { return a + std::to_string(b) + " "; }) 
            << " and k = " << std::get<1>(ele);
    }
}

TEST_F(ArrayTest, SpiralMatrix)
{
  std::vector<std::tuple<std::vector<std::vector<int>>, std::vector<int>>> tests =
  {
    { { {1,2,3}, {4,5,6}, {7,8,9} }, {1,2,3,6,9,8,7,4,5} },
    { { {1,2,3,4}, {5,6,7,8}, {9,10,11,12} }, {1,2,3,4,8,12,11,10,9,5,6,7} }
  };

  for(auto test : tests)
  {
    auto ret = spiral_matrix(std::get<0>(test));
    ASSERT_EQ(ret.size(), std::get<1>(test).size());
    for(size_t i = 0; i < ret.size(); ++i)
    {
      ASSERT_EQ(ret[i], std::get<1>(test)[i]);
    }
  }
}