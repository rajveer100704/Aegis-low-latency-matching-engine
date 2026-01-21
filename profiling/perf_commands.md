\# Performance Profiling Commands



\## CPU Profiling

perf record -F 99 -g -- ./load\_test

perf report



\## Flamegraph Generation

perf script > out.perf

stackcollapse-perf.pl out.perf > out.folded

flamegraph.pl out.folded > flamegraph.svg



\## Cache Analysis

perf stat -e cache-misses,cache-references ./load\_test



\## Branch Prediction

perf stat -e branch-misses,branches ./load\_test



\## Scheduler Latency

perf sched record ./load\_test

perf sched latency



