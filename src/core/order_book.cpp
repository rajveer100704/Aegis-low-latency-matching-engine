#include "order_book.hpp"

inline int price_to_index(double price) {
    return static_cast<int>(price);
}

void OrderBook::add_order(const Order& order) {
    int idx = price_to_index(order.price);

    if (order.side == static_cast<uint8_t>(Side::BUY)) {
        auto& lvl = bids[idx];
        lvl.orders[lvl.tail++] = order;
        if (best_bid < idx) best_bid = idx;
    } else {
        auto& lvl = asks[idx];
        lvl.orders[lvl.tail++] = order;
        if (best_ask == -1 || idx < best_ask) best_ask = idx;
    }
}

Order* OrderBook::best_buy() {
    return (best_bid >= 0)
        ? &bids[best_bid].orders[bids[best_bid].head]
        : nullptr;
}

Order* OrderBook::best_sell() {
    return (best_ask >= 0)
        ? &asks[best_ask].orders[asks[best_ask].head]
        : nullptr;
}

void OrderBook::pop_best_buy() {
    auto& lvl = bids[best_bid];
    if (++lvl.head == lvl.tail)
        best_bid = -1;
}

void OrderBook::pop_best_sell() {
    auto& lvl = asks[best_ask];
    if (++lvl.head == lvl.tail)
        best_ask = -1;
}
