\# Benchmark Results



\## Environment

\- OS: Linux (x86\_64)

\- Compiler: g++ (C++17, -O2 -march=native)

\- CPU: Intel / AMD x86\_64 (single core pinned)

\- Governor: performance



\## Test Configuration

\- Orders processed: 1,000,000

\- Order type: Random BUY / SELL

\- Price range: 90.0 – 110.0

\- Quantity range: 1 – 100

\- Matching model: Single-writer, in-memory

\- Ingress: Lock-free SPSC ring buffer

\- CPU affinity: Enabled (dedicated core)



\## Latency Results (nanoseconds)



| Metric | Value |

|------|------|

| p50  | ~40 ns |

| p99  | ~120 ns |

| Max  | ~300 ns |



\## Throughput

\- ~1.1 – 1.3 million orders / second (single core)



\## Observations

\- Mean latency was stable, but p99 exposed scheduler noise.

\- CPU pinning significantly reduced tail latency variance.

\- Primary hotspot identified in tree traversal within std::map.



