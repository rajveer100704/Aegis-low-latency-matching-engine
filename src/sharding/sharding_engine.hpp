#pragma once
#include "../core/matching_engine.hpp"
#include <unordered_map>
#include <memory>

class ShardedMatchingEngine {
public:
    MatchingEngine& engine(const std::string& sym) {
        if (!engines_[sym]) {
            books_[sym] = OrderBook();
            engines_[sym] = std::make_unique<MatchingEngine>(books_[sym]);
        }
        return *engines_[sym];
    }

private:
    std::unordered_map<std::string, OrderBook> books_;
    std::unordered_map<std::string,
        std::unique_ptr<MatchingEngine>> engines_;
};
