#include "../src/core/order_book.hpp"
#include "../src/core/matching_engine.hpp"
#include <iostream>
#include <cstdint>

// -------- RDTSC --------
static inline uint64_t rdtsc() {
    unsigned int lo, hi;
    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

int main() {
    OrderBook book;
    MatchingEngine engine(book);

    // Warm-up book
    for (int i = 0; i < 128; ++i) {
        Order seed {
            static_cast<uint64_t>(i),
            0,
            100.0,
            10,
            static_cast<uint8_t>(Side::SELL)
        };
        book.add_order(seed);
    }

    Trade trades[8];

    constexpr int N = 1'000'000;
    uint64_t cycles = 0;

    for (int i = 0; i < N; ++i) {
        Order o {
            static_cast<uint64_t>(1000 + i),
            static_cast<uint64_t>(i),
            100.0,
            10,
            static_cast<uint8_t>(Side::BUY)
        };

        uint64_t start = rdtsc();
        engine.process(o, trades);
        uint64_t end = rdtsc();

        cycles += (end - start);
    }

    double avg_cycles = static_cast<double>(cycles) / N;
    std::cout << "Avg cycles per order: " << avg_cycles << "\n";
    return 0;
}
