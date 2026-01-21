#include "core/order_book.hpp"
#include "core/matching_engine.hpp"
#include "infra/cpu_affinity.hpp"
#include "metrics/metrics.hpp"

#include <chrono>
#include <cstdint>

int main() {
    // 🔒 Pin to isolated core
    pin_thread_to_core(2);

    OrderBook book;
    MatchingEngine engine(book);
    Metrics metrics;

    // 🔥 Warm up book (critical for cache realism)
    for (int i = 0; i < 128; ++i) {
        Order seed {
            static_cast<uint64_t>(i),
            0,                  // timestamp
            100.0,              // price
            10,                 // quantity
            static_cast<uint8_t>(Side::SELL)
        };
        book.add_order(seed);
    }

    // 🔥 Stack-allocated trade buffer (NO heap)
    Trade trades[8];

    constexpr int N = 100000;

    for (int i = 0; i < N; ++i) {
        Order incoming {
            static_cast<uint64_t>(1000 + i),
            static_cast<uint64_t>(i),
            100.0,
            10,
            static_cast<uint8_t>(Side::BUY)
        };

        auto start = std::chrono::steady_clock::now();

        // 🚀 HOT PATH
        engine.process(incoming, trades);

        auto end = std::chrono::steady_clock::now();

        metrics.record(
            std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
        );
    }

    metrics.report();
    return 0;
}
