#pragma once
#include <vector>
#include <limits>
#include <algorithm>

class BestTimeSell
{
   enum class Operation
   {
      Buy = 0,
      Sell,
      Hold
   };

public:
   static int maxProfit(const std::vector<int>& prices) 
   { //This is basically sliding window
      int min_price = std::numeric_limits<int>::max(), max_profit = 0;

      for(auto price : prices) {
         if(price < min_price)              min_price  = price;
         if(price - min_price > max_profit) max_profit = price - min_price;
      }

      return max_profit;
   }

   //https://leetcode.com/problems/best-time-to-buy-and-sell-stock-ii/description/
   static int maxProfit2(const std::vector<int>& prices) 
   {
      int max_profit = 0;

      do_maxProfit2(prices, 1, prices[0], (-1 * prices[0]), max_profit, Operation::Buy);
      do_maxProfit2(prices, 1, prices[0], (-1 * prices[0]), max_profit, Operation::Sell);
      do_maxProfit2(prices, 1, prices[0], (-1 * prices[0]), max_profit, Operation::Hold);

      do_maxProfit2(prices, 1, -1, 0, max_profit, Operation::Buy);
      do_maxProfit2(prices, 1, -1, 0, max_profit, Operation::Sell);
      do_maxProfit2(prices, 1, -1, 0, max_profit, Operation::Hold);
      
      return max_profit;
   }

   //https://leetcode.com/problems/best-time-to-buy-and-sell-stock-iv/
   int maxProfitK(int k, const std::vector<int>& prices) 
   {
      return 0;
   }
   
private:
   static void do_maxProfit2(const std::vector<int>& prices, int index, int cur_stock, int cur_profit, int& max_profit, Operation Op)
   {
      if(index >= prices.size() - 1) return;

      switch(Op)
      {
         case Operation::Buy:
            if(cur_stock >= 0) cur_profit += prices[index];
            cur_stock   = prices[index];
            cur_profit -= cur_stock;

            if(cur_profit > max_profit) max_profit = cur_profit;         
            break;

         case Operation::Sell:
            if(cur_stock >= 0) cur_profit += prices[index];
            cur_stock = -1;

            if(cur_profit > max_profit) max_profit = cur_profit; 
            break;

         case Operation::Hold:
            break;
      }

      do_maxProfit2(prices, index+1, cur_stock, cur_profit, max_profit, Operation::Buy);
      do_maxProfit2(prices, index+1, cur_stock, cur_profit, max_profit, Operation::Sell);
      do_maxProfit2(prices, index+1, cur_stock, cur_profit, max_profit, Operation::Hold);
   }
};