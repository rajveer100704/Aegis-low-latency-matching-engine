#pragma once
#include "order.hpp"

static constexpr int MAX_LEVELS = 1024;
static constexpr int MAX_ORDERS_PER_LEVEL = 64;

struct PriceLevel {
    Order orders[MAX_ORDERS_PER_LEVEL];
    uint32_t head = 0;
    uint32_t tail = 0;
};

class OrderBook {
public:
    PriceLevel bids[MAX_LEVELS];
    PriceLevel asks[MAX_LEVELS];

    int best_bid = -1;
    int best_ask = -1;

    void add_order(const Order& order);
    Order* best_buy();
    Order* best_sell();
    void pop_best_buy();
    void pop_best_sell();
};
