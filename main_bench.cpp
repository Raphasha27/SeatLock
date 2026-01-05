#include <iostream>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include "AtomicSeatManager.h"

// Stress Test for Lock-Free Engine
void stressWorker(AtomicSeatManager& manager, int userId, int operations) {
    for (int i = 0; i < operations; ++i) {
        // Randomly pick a seat 1-1000
        int seatId = (rand() % 1000) + 1;
        
        // 50% chance to hold, 25% to confirm, 25% to release
        int action = rand() % 4;
        
        if (action <= 1) { 
            manager.holdSeat(seatId, userId, 500);
        } else if (action == 2) {
            manager.confirmSeat(seatId, userId);
        } else {
            manager.releaseSeat(seatId);
        }
    }
}

int main() {
    const int NUM_SEATS = 1000;
    const int THREADS = 16;
    const int OPS_PER_THREAD = 100000;

    std::cout << "Initializing Lock-Free Manager for " << NUM_SEATS << " seats.\n";
    AtomicSeatManager manager(NUM_SEATS);

    std::cout << "Starting " << THREADS << " threads performing " << OPS_PER_THREAD << " ops each...\n";
    
    auto start = std::chrono::steady_clock::now();
    
    std::vector<std::thread> workers;
    for (int i = 0; i < THREADS; ++i) {
        workers.emplace_back(stressWorker, std::ref(manager), i+1, OPS_PER_THREAD);
    }

    for (auto& t : workers) {
        t.join();
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    
    double totalOps = (double)THREADS * OPS_PER_THREAD;
    double opsPerSec = totalOps / elapsed.count();

    std::cout << "Benchmark Finished:\n";
    std::cout << "  Time: " << elapsed.count() << "s\n";
    std::cout << "  Ops/Sec: " << opsPerSec << "\n";

    return 0;
}
