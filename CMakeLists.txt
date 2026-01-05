cmake_minimum_required(VERSION 3.10)
project(SeatLock)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Standard Lock-Based Simulation
add_executable(SeatLock 
    src/main.cpp 
    src/SeatManager.cpp
    src/ExpiryWorker.cpp
)

# Lock-Free Comparison Benchmark
add_executable(SeatLockBench
    main_bench.cpp
    src/AtomicSeatManager.cpp
)
