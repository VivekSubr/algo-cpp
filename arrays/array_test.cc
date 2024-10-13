#include "array_test.h"
#include "text_justify.h"
#include <tuple>

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