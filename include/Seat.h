#pragma once
#include <string>
#include <chrono>

enum class SeatStatus {
    AVAILABLE,
    HELD,
    SOLD
};

struct Seat {
    int seatId;
    SeatStatus status;
    std::string userId;
    std::chrono::steady_clock::time_point holdExpiry;

    Seat() : seatId(0), status(SeatStatus::AVAILABLE), userId(""), holdExpiry() {}
    Seat(int id) : seatId(id), status(SeatStatus::AVAILABLE), userId(""), holdExpiry() {}
};
