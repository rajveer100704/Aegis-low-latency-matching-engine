\# Architecture Design



This document describes the architectural decisions behind the

\*\*Low-Latency In-Memory Order Matching Engine\*\*.  

The system is designed to mirror real-world low-latency trading

infrastructure while remaining simple, deterministic, and explainable.



---



\## Design Goals



The primary goals of this architecture are:



1\. \*\*Deterministic execution\*\*

2\. \*\*Low and predictable latency\*\*

3\. \*\*Clear concurrency boundaries\*\*

4\. \*\*In-memory data paths\*\*

5\. \*\*Strong observability and profiling\*\*



The system intentionally prioritizes correctness and clarity before

aggressive optimization.



---



\## High-Level Overview



The system is composed of three logical layers:



1\. \*\*Ingress Layer\*\* – Handles concurrency and order submission

2\. \*\*Matching Core\*\* – Performs deterministic order matching

3\. \*\*Observation Layer\*\* – Measures latency and performance



Only the ingress layer is concurrent.  

The matching core remains single-threaded.



---



\## Single-Writer Matching Engine



The matching engine follows a \*\*single-writer model\*\*:



\- Only one thread mutates the order book

\- No locks are used in the hot matching path

\- Execution order is deterministic



This design:

\- Simplifies correctness reasoning

\- Eliminates lock contention in the core

\- Mirrors real exchange and HFT matching engines



Determinism is favored over parallelism.



---



\## Order Book Design



Each order book maintains:



\- A \*\*buy side\*\* ordered by descending price

\- A \*\*sell side\*\* ordered by ascending price

\- FIFO ordering within the same price level



\### Key properties:

\- Enforces \*\*price-time priority\*\*

\- Entirely in-memory

\- Predictable complexity



The data structure choice prioritizes:

\- Correctness

\- Simplicity

\- Ease of profiling and reasoning



---



\## Concurrency Model



Concurrency is \*\*explicitly isolated\*\* at the system boundary.



\### Ingress

\- Orders are submitted through a queue

\- Two implementations exist:

&nbsp; - Mutex-based `ThreadSafeQueue` (baseline)

&nbsp; - Lock-free SPSC ring buffer (optimized)



\### Core

\- Matching engine consumes orders sequentially

\- No locks or atomics inside the matching logic



This separation ensures that concurrency does not leak into the hot path.



---



\## Lock-Free Ingress



The lock-free queue:

\- Is bounded

\- Uses single-producer / single-consumer semantics

\- Reduces tail latency caused by mutex contention



This queue is used \*\*only at the ingress boundary\*\*, never inside the

matching engine.



---



\## CPU Affinity



The matching thread is pinned to a dedicated CPU core.



Benefits:

\- Reduced context switches

\- Improved cache locality

\- More stable p99 latency



CPU affinity is treated as a performance optimization, not a correctness

requirement.



---



\## Latency Measurement



Latency is measured per order using a monotonic clock.



Metrics collected:

\- p50 latency

\- p99 latency

\- Maximum latency



Mean latency is intentionally not emphasized, as it often hides tail

behavior.



---



\## Sharding Strategy



The system supports \*\*per-symbol sharding\*\*:



\- Each symbol has an independent order book

\- Each book preserves single-writer semantics

\- Enables horizontal scalability without cross-book locks



Sharding allows safe parallelism while maintaining determinism per symbol.



---



\## Profiling \& Observability



Performance analysis is a first-class concern.



Tools used:

\- Linux `perf`

\- Flamegraphs

\- Cache and branch statistics



Profiling results guide optimization decisions rather than assumptions.



---



\## Trade-Offs \& Limitations



Intentional trade-offs include:

\- Using tree-based structures instead of flat arrays

\- Single-threaded matching per book

\- No persistence or recovery logic



These decisions keep the system focused on \*\*low-latency core mechanics\*\*

rather than full exchange completeness.



---



\## Summary



This architecture reflects real-world low-latency system design:



\- Simple, deterministic core

\- Explicit concurrency boundaries

\- Measured performance

\- Optimization driven by data



The design favors \*\*engineering discipline\*\* over premature complexity.

\# Architecture Design



This document describes the architectural decisions behind the

\*\*Low-Latency In-Memory Order Matching Engine\*\*.  

The system is designed to mirror real-world low-latency trading

infrastructure while remaining simple, deterministic, and explainable.



---



\## Design Goals



The primary goals of this architecture are:



1\. \*\*Deterministic execution\*\*

2\. \*\*Low and predictable latency\*\*

3\. \*\*Clear concurrency boundaries\*\*

4\. \*\*In-memory data paths\*\*

5\. \*\*Strong observability and profiling\*\*



The system intentionally prioritizes correctness and clarity before

aggressive optimization.



---



\## High-Level Overview



The system is composed of three logical layers:



1\. \*\*Ingress Layer\*\* – Handles concurrency and order submission

2\. \*\*Matching Core\*\* – Performs deterministic order matching

3\. \*\*Observation Layer\*\* – Measures latency and performance



Only the ingress layer is concurrent.  

The matching core remains single-threaded.



---



\## Single-Writer Matching Engine



The matching engine follows a \*\*single-writer model\*\*:



\- Only one thread mutates the order book

\- No locks are used in the hot matching path

\- Execution order is deterministic



This design:

\- Simplifies correctness reasoning

\- Eliminates lock contention in the core

\- Mirrors real exchange and HFT matching engines



Determinism is favored over parallelism.



---



\## Order Book Design



Each order book maintains:



\- A \*\*buy side\*\* ordered by descending price

\- A \*\*sell side\*\* ordered by ascending price

\- FIFO ordering within the same price level



\### Key properties:

\- Enforces \*\*price-time priority\*\*

\- Entirely in-memory

\- Predictable complexity



The data structure choice prioritizes:

\- Correctness

\- Simplicity

\- Ease of profiling and reasoning



---



\## Concurrency Model



Concurrency is \*\*explicitly isolated\*\* at the system boundary.



\### Ingress

\- Orders are submitted through a queue

\- Two implementations exist:

&nbsp; - Mutex-based `ThreadSafeQueue` (baseline)

&nbsp; - Lock-free SPSC ring buffer (optimized)



\### Core

\- Matching engine consumes orders sequentially

\- No locks or atomics inside the matching logic



This separation ensures that concurrency does not leak into the hot path.



---



\## Lock-Free Ingress



The lock-free queue:

\- Is bounded

\- Uses single-producer / single-consumer semantics

\- Reduces tail latency caused by mutex contention



This queue is used \*\*only at the ingress boundary\*\*, never inside the

matching engine.



---



\## CPU Affinity



The matching thread is pinned to a dedicated CPU core.



Benefits:

\- Reduced context switches

\- Improved cache locality

\- More stable p99 latency



CPU affinity is treated as a performance optimization, not a correctness

requirement.



---



\## Latency Measurement



Latency is measured per order using a monotonic clock.



Metrics collected:

\- p50 latency

\- p99 latency

\- Maximum latency



Mean latency is intentionally not emphasized, as it often hides tail

behavior.



---



\## Sharding Strategy



The system supports \*\*per-symbol sharding\*\*:



\- Each symbol has an independent order book

\- Each book preserves single-writer semantics

\- Enables horizontal scalability without cross-book locks



Sharding allows safe parallelism while maintaining determinism per symbol.



---



\## Profiling \& Observability



Performance analysis is a first-class concern.



Tools used:

\- Linux `perf`

\- Flamegraphs

\- Cache and branch statistics



Profiling results guide optimization decisions rather than assumptions.



---



\## Trade-Offs \& Limitations



Intentional trade-offs include:

\- Using tree-based structures instead of flat arrays

\- Single-threaded matching per book

\- No persistence or recovery logic



These decisions keep the system focused on \*\*low-latency core mechanics\*\*

rather than full exchange completeness.



---



\## Summary



This architecture reflects real-world low-latency system design:



\- Simple, deterministic core

\- Explicit concurrency boundaries

\- Measured performance

\- Optimization driven by data



The design favors \*\*engineering discipline\*\* over premature complexity.



