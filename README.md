# ⚔️ Aegis Matching Engine

A production-style, **ultra–low latency trading matching engine** designed with a  
**single-writer, event-driven architecture** and optimized at the **CPU-cycle level**.

This project focuses on **determinism, correctness, and latency predictability** —  
the same constraints faced by real-world electronic trading systems.


---

## 🔑 Key Properties

- ~940 ns average matching latency
- ~55 CPU cycles per order
- Single-writer, multi-reader event-driven design
- Lock-free hot path (no mutexes in matching logic)
- Strict price-time (FIFO) priority
- CPU-affinity pinning to reduce cache contention
- Flame-graph and `rdtsc` verified performance

---

## 🏗️ Architecture Overview

Orders are ingested through a deterministic event loop and processed by a  
**single-writer order book**, ensuring predictable execution and strict FIFO  
price-time priority.

The engine is intentionally **core-local** to avoid cross-core cache invalidation  
and synchronization overhead.

Benchmarking and profiling are isolated from the matching path to preserve  
measurement fidelity.

Detailed design notes:  
docs/architecture.md

---

## 📂 Project Structure

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
    └── README.md

This structure cleanly separates **measurement from behavior**, allowing the  
matching engine to remain minimal, deterministic, and cache-friendly.

---

## 📊 Performance Results

All performance figures were measured on a **pinned CPU core** under steady-state  
load using **cycle-accurate instrumentation**.

    Metric                    Value
    ------------------------- -------------------------
    Average latency           ~940 ns
    CPU cycles per order      ~55
    Synchronization           Lock-free hot path
    Tooling                  perf, flame graphs, rdtsc

Full benchmark data:  
results/benchmark_results.md

These numbers reflect **steady-state behavior**, not best-case microbenchmarks.

---

## 🔬 Profiling & Validation

The matching hot path was analyzed using **Linux perf** with DWARF call graphs and  
validated with **cycle-level timing via rdtsc**.

Observed behavior:

- Matching logic dominates CPU time
- No mutex contention on the hot path
- Page faults limited to initialization
- Flat, stable flame graphs under sustained load

This confirms the engine is **compute-bound and predictable**, not scheduler-  
or synchronization-bound.

---

## 🔥 Profiling Evidence

The following profiling artifacts are included as **evidence**, not illustration.


### CPU Hot Path

Flame graphs show the matching engine consuming the majority of execution time,  
with minimal framework or syscall overhead.
<img width="1843" height="688" alt="CPU Hot Path" src="https://github.com/user-attachments/assets/11b4c1a2-bf12-495e-8695-c78e7741f55a" />

### Instruction Cycles per Order

Cycle counts measured using `rdtsc` confirm ~55 cycles per order in steady state.
<img width="1881" height="392" alt="Instruction Cycles" src="https://github.com/user-attachments/assets/284d905d-2435-430e-8098-9b12f7784876" />

### Branch & Cache Behavior
<img width="1859" height="825" alt="Branch + Cache" src="https://github.com/user-attachments/assets/21703bcf-358b-4064-87eb-f66fe87cb1ab" />

### Latency & Throughput
1.
<img width="1906" height="963" alt="Screenshot 2026-01-07 083137" src="https://github.com/user-attachments/assets/f14e10e2-c040-4939-8157-a276342357ea" />

2.
<img width="1717" height="917" alt="Screenshot 2026-01-07 100755" src="https://github.com/user-attachments/assets/48bee429-6208-47bd-8ecf-fe618381bc50" />

3.
<img width="1268" height="300" alt="Screenshot 2026-01-07 101711" src="https://github.com/user-attachments/assets/7c35c208-a88a-46d5-9bd7-5edafb827b5c" />


Cache-friendly access patterns with minimal branch misprediction under load.

---

## 🛠️ Build & Run

### Requirements

- Linux (tested on WSL2 / Ubuntu)
- GCC 11+
- CMake 3.16+
- perf (for profiling)

### Build

    mkdir build && cd build

    cmake .. \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_CXX_FLAGS="-O3 -march=native -flto -fno-omit-frame-pointer"

    make -j$(nproc)

### Run Benchmark

    ./load_test

Example output:

    Avg latency: 941 ns
    Avg cycles per order: ~55

---

## 🧠 Design Philosophy

- Determinism over parallelism
- Correctness before throughput
- Measure before optimizing
- Avoid abstraction on hot paths
- CPU reality beats theoretical performance

This project intentionally trades raw throughput for **predictability and  
measurable behavior**, reflecting real-world exchange design constraints.

---

