#include "matching_engine.hpp"

int MatchingEngine::process(Order& incoming, Trade* out) {
    int trade_count = 0;
    const bool is_buy = (incoming.side == static_cast<uint8_t>(Side::BUY));

    while (incoming.quantity > 0) {
        Order* best = is_buy
            ? book_.best_sell()
            : book_.best_buy();

        if (!best) break;

        if ((is_buy && best->price > incoming.price) ||
            (!is_buy && best->price < incoming.price))
            break;

        uint32_t qty =
            incoming.quantity < best->quantity
                ? incoming.quantity
                : best->quantity;

        out[trade_count++] = {
            is_buy ? incoming.order_id : best->order_id,
            is_buy ? best->order_id : incoming.order_id,
            best->price,
            qty,
            incoming.timestamp
        };

        incoming.quantity -= qty;
        best->quantity -= qty;

        if (best->quantity == 0) {
            is_buy ? book_.pop_best_sell()
                   : book_.pop_best_buy();
        }
    }

    if (incoming.quantity > 0)
        book_.add_order(incoming);

    return trade_count;
}
