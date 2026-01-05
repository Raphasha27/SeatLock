#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include "SeatManager.h"
#include "ExpiryWorker.h"

void simulateUser(SeatManager& manager, int userId, int numSeats) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> seatDist(1, numSeats);
    std::uniform_int_distribution<> actionDist(0, 5); // 0=Release, 1-3=Hold, 4-5=Confirm

    for (int i = 0; i < 50; ++i) {
        int seatId = seatDist(gen);
        int action = actionDist(gen);
        std::string userStr = "User" + std::to_string(userId);

        if (action <= 3) {
            // Try to hold
            bool success = manager.holdSeat(seatId, userStr, 2); // 2 second hold
            if (success) {
                // std::cout << userStr << " HELD seat " << seatId << "\n";
            }
        } else if (action == 4) {
            // Try to confirm
            bool success = manager.confirmSeat(seatId, userStr);
            if (success) {
                // std::cout << userStr << " CONFIRMED seat " << seatId << "\n";
            }
        } else {
             // Release just in case (though we usually release if we change mind or expiry)
             // manager.releaseSeat(seatId);
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {
    const int NUM_SEATS = 100;
    const int NUM_USERS = 20;

    std::cout << "Initializing SeatLock Engine with " << NUM_SEATS << " seats...\n";
    SeatManager manager(NUM_SEATS);

    std::cout << "Starting ExpiryWorker...\n";
    ExpiryWorker expiryWorker(manager);
    expiryWorker.start();

    std::cout << "Starting simulation with " << NUM_USERS << " concurrent users...\n";
    std::vector<std::thread> threads;
    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < NUM_USERS; ++i) {
        threads.emplace_back(simulateUser, std::ref(manager), i, NUM_SEATS);
    }

    for (auto& t : threads) {
        t.join();
    }

    auto end = std::chrono::steady_clock::now();
    std::chrono::duration<double> elapsed = end - start;

    std::cout << "Simulation complete in " << elapsed.count() << " seconds.\n";

    // Stop worker
    expiryWorker.stop();

    // Print final stats
    auto states = manager.getSeatStates();
    int sold = 0;
    int held = 0;
    int available = 0;
    for (const auto& s : states) {
        if (s.status == SeatStatus::SOLD) sold++;
        else if (s.status == SeatStatus::HELD) held++;
        else available++;
    }

    std::cout << "Final State:\n";
    std::cout << "  SOLD: " << sold << "\n";
    std::cout << "  HELD: " << held << "\n";
    std::cout << "  AVAILABLE: " << available << "\n";

    return 0;
}
