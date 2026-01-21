#pragma once
#include <cstdint>

enum class Side : uint8_t { BUY = 0, SELL = 1 };

struct alignas(64) Order {
    uint64_t order_id;
    uint64_t timestamp;
    double   price;
    uint32_t quantity;
    uint8_t  side;
    uint8_t  _pad[35]; // force single cache line
};
