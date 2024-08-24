#pragma once
#include <vector>
#include <limits>
#include <algorithm>
#include <memory>
#include <variant>
#include <iostream>

class BestTimeSell
{
   enum class Operation
   {
      None = 0,
      Buy,
      Sell,
      Hold
   };

   struct stock_state 
   {
      virtual ~stock_state() = default;
      virtual std::shared_ptr<stock_state> on_update(Operation op, int cp, int& mp) = 0; //Operating on next stock, return next state

      Operation current_op = Operation::None; 
      int m_nTransactions; 
      int m_price;
      int m_cur_profit = 0;
   };

   struct buying_stock : stock_state
   {
      buying_stock(int nTransactions, int price, int cur_profit)
      {
         current_op      = Operation::Buy;
         m_nTransactions = nTransactions;
         m_price         = price;
         m_cur_profit    = cur_profit;
      }

      buying_stock(std::shared_ptr<stock_state> state, int price)
      {
         current_op      = Operation::Buy;
         m_nTransactions = state->m_nTransactions;
         m_price         = price;
         m_cur_profit    = state->m_cur_profit;
      }

      std::shared_ptr<stock_state> on_update(Operation op, int new_price, int& max_profit) override 
      {
         switch(op)
         {
            case Operation::Buy:
               return nullptr; //Can't go from buy to buy

            case Operation::Sell:
               --m_nTransactions; 
               m_cur_profit += m_price;
               if(m_cur_profit > max_profit) max_profit = m_cur_profit;
               return std::make_shared<selling_stock>(m_nTransactions, m_cur_profit);
                 
            case Operation::Hold:
                 return std::make_shared<selling_stock>(m_nTransactions, m_price, m_cur_profit);
         }
      }
   };

   struct selling_stock : stock_state
   {
      selling_stock(int nTransactions, int cur_profit)
      {
         current_op      = Operation::Sell;
         m_nTransactions = nTransactions;
         m_price         = -1;
         m_cur_profit    = cur_profit;
      }

      selling_stock(std::shared_ptr<stock_state> state)
      {
         current_op      = Operation::Buy;
         m_nTransactions = state->m_nTransactions;
         m_price         = -1;
      }

      std::shared_ptr<stock_state> on_update(Operation op, int new_price, int& max_profit) override 
      {
         switch(op)
         {
            case Operation::Buy:
               m_cur_profit = m_cur_profit - new_price;
               return std::make_shared<buying_stock>(m_nTransactions, new_price, m_cur_profit);

            case Operation::Sell:
               return nullptr; //Can't go from sell to sell

            case Operation::Hold:
               return std::make_shared<selling_stock>();
         }
      }
   };

   struct holding_stock : stock_state
   {
      holding_stock()
      {
         current_op = Operation::Hold;
      }

      std::shared_ptr<stock_state> on_update(Operation op, int new_price, int& max_profit) override 
      {
         switch(op)
         {
            case Operation::Buy:
                 return std::make_shared<buying_stock>(m_nTransactions, m_price);

            case Operation::Sell:
                 //return std::make_shared<selling_stock>(m_nTransactions, new_price, m_cur_profit);

            case Operation::Hold:
                 return std::make_shared<selling_stock>(m_nTransactions, m_price);
         }
      }
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
   /*
      The complexity here is that it adds the concept of 'transaction' - one buy + sell operation together.
      Find max profit, constrained to k transactions.

      We implement a small state machine here, with states being Operation enum, ie the last op done.
      Depending on current state,
      Buy  --> Sell, or Hold (Can't buy, Sell with complete one transactino)
      Sell --> Buy or Hold. 
      Hold --> Sell, Buy or Hold.
   */
   static int maxProfitK(int k, const std::vector<int>& prices) 
   {
      int max_profit = 0;

      do_maxProfitK(std::make_shared<buying_stock>(k, prices[0]), prices, 1, max_profit);

      return max_profit;
   }
   
private:
   static void do_maxProfit2(const std::vector<int>& prices, int index, int cur_stock, int cur_profit, int& max_profit, Operation Op)
   {
      if(index >= prices.size()) return;

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

   static void do_maxProfitK(std::shared_ptr<stock_state> state, const std::vector<int>& prices, int index, int& max_profit)
   {
      if(index <= prices.size()) return;

      int nPrice = -1;
      if((index+1) < prices.size()) nPrice = prices[index+1];
      
      switch(state->current_op)
      {
         case Operation::Buy: 
         {
            auto sell_state = state->on_update(Operation::Sell, nPrice, max_profit);
            do_maxProfitK(sell_state, prices, index+1, max_profit);

            auto hold_state = state->on_update(Operation::Hold, nPrice, max_profit);
            do_maxProfitK(hold_state, prices, index+1, max_profit);
         } break;

         case Operation::Sell:
            break;

         case Operation::Hold:
            break;
      }
   }
};