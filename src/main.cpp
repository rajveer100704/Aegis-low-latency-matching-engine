#include "core/order_book.hpp"
#include "core/matching_engine.hpp"
#include <iostream>

int main() {
    OrderBook book;
    MatchingEngine engine(book);

    Trade trades[8];

    Order o {
        1,
        0,
        100.0,
        10,
        static_cast<uint8_t>(Side::BUY)
    };

    engine.process(o, trades);
    std::cout << "OK\n";
}
