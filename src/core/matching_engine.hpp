#pragma once
#include "order_book.hpp"
#include "trade.hpp"
#include <algorithm>

class MatchingEngine {
public:
    explicit MatchingEngine(OrderBook& book)
        : book_(book) {}

    inline int process(Order& incoming, Trade* out);

private:
    OrderBook& book_;
};
