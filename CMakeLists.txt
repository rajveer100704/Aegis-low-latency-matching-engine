cmake_minimum_required(VERSION 3.10)
project(low_latency_matching_engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(matching_engine
    src/main.cpp
    src/core/order_book.cpp
    src/core/matching_engine.cpp
)

add_executable(load_test
    benchmarks/load_test.cpp
    src/core/order_book.cpp
    src/core/matching_engine.cpp
)

if (MSVC)
    target_compile_options(matching_engine PRIVATE /O2)
    target_compile_options(load_test PRIVATE /O2)
else()
    target_compile_options(matching_engine PRIVATE -O3 -march=native)
    target_compile_options(load_test PRIVATE -O3 -march=native)
endif()
