#pragma once
#include "Seat.h"
#include <vector>
#include <unordered_map>
#include <mutex>
#include <memory>
#include <string>

struct SeatContainer {
    Seat seat;
    std::mutex mtx;
    SeatContainer(int id) : seat(id) {}
};

class SeatManager {
public:
    SeatManager(int totalSeats);

    bool holdSeat(int seatId, const std::string& userId, int holdDurationSeconds);
    bool confirmSeat(int seatId, const std::string& userId);
    bool releaseSeat(int seatId);
    void cleanupExpiredHolds();

    struct SeatInfo {
        int seatId;
        SeatStatus status;
        std::string userId;
    };
    std::vector<SeatInfo> getSeatStates();

private:
    // Using pointers to ensure SeatContainer (and its mutex) is stable in memory
    std::unordered_map<int, std::unique_ptr<SeatContainer>> seats;
};
