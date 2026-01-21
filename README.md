# Aegis Matching Engine

A production-style, **ultra–low latency trading matching engine** designed with a  
**single-writer, event-driven architecture** and optimized at the **CPU-cycle level**.

This project focuses on **determinism, correctness, and latency predictability** —  
the same constraints faced by real-world electronic trading systems.


---

## Key Properties

- **~940 ns average matching latency**
- **~55 CPU cycles per order**
- Single-writer, multi-reader event-driven design
- Lock-free hot path (no mutexes in matching logic)
- Strict price-time (FIFO) priority
- CPU-affinity pinning to reduce cache contention
- Flame-graph and `rdtsc` verified performance

---

## Architecture Overview

Orders are ingested through a deterministic event loop and processed by a  
**single-writer order book**, ensuring predictable execution and strict FIFO  
price-time priority.

The engine is intentionally **core-local** to avoid cross-core cache invalidation  
and synchronization overhead. Benchmarking and profiling are isolated from the  
matching path to preserve signal fidelity.

📄 Detailed design notes: `docs/architecture.md`

---

## Project Structure

```text
.
├── benchmarks/        # Synthetic load generator
├── docs/              # Architecture & design notes
├── profiling/         # perf commands & methodology
│   └── images/        # Flame graphs & profiling evidence
├── results/           # Benchmark summaries
├── src/
│   ├── core/          # Order book & matching logic
│   ├── infra/         # Event loop & execution model
│   ├── metrics/       # Latency & cycle tracking
│   ├── sharding/      # Instrument partitioning (future)
│   └── main.cpp
├── CMakeLists.txt


