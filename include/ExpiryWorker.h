#pragma once
#include "SeatManager.h"
#include <thread>
#include <atomic>

class ExpiryWorker {
public:
    ExpiryWorker(SeatManager& manager);
    ~ExpiryWorker();

    void start();
    void stop();

private:
    void run();

    SeatManager& seatManager;
    std::thread workerThread;
    std::atomic<bool> running;
};
