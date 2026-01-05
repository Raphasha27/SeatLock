#include "ExpiryWorker.h"
#include <chrono>

ExpiryWorker::ExpiryWorker(SeatManager& manager) 
    : seatManager(manager), running(false) {}

ExpiryWorker::~ExpiryWorker() {
    stop();
}

void ExpiryWorker::start() {
    if (running) return;
    running = true;
    workerThread = std::thread(&ExpiryWorker::run, this);
}

void ExpiryWorker::stop() {
    if (!running) return;
    running = false;
    if (workerThread.joinable()) {
        workerThread.join();
    }
}

void ExpiryWorker::run() {
    while (running) {
        seatManager.cleanupExpiredHolds();
        // Sleep for a short interval to avoid spinning too fast
        // In a real system, this might be tunable or event-driven
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
