#pragma once

#include <thread>

#if defined(_WIN32)

#include <windows.h>

inline void pin_thread_to_core(size_t core_id) {
    DWORD_PTR mask = (static_cast<DWORD_PTR>(1) << core_id);
    HANDLE thread = GetCurrentThread();
    SetThreadAffinityMask(thread, mask);
}

#elif defined(__linux__)

#include <pthread.h>
#include <sched.h>

inline void pin_thread_to_core(size_t core_id) {
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(core_id, &cpuset);

    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
}

#else

inline void pin_thread_to_core(size_t) {
    // Unsupported platform – no-op
}

#endif
