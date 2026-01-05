#include "SeatManager.h"
#include <chrono>
#include <iostream>

SeatManager::SeatManager(int totalSeats) {
    for (int i = 1; i <= totalSeats; ++i) {
        seats[i] = std::make_unique<SeatContainer>(i);
    }
}

bool SeatManager::holdSeat(int seatId, const std::string& userId, int holdDurationSeconds) {
    auto it = seats.find(seatId);
    if (it == seats.end()) return false;

    std::lock_guard<std::mutex> lock(it->second->mtx);
    Seat& seat = it->second->seat;

    auto now = std::chrono::steady_clock::now();

    // Check if hold is expired
    if (seat.status == SeatStatus::HELD && now > seat.holdExpiry) {
        seat.status = SeatStatus::AVAILABLE;
        seat.userId = "";
    }

    if (seat.status == SeatStatus::AVAILABLE) {
        seat.status = SeatStatus::HELD;
        seat.userId = userId;
        seat.holdExpiry = now + std::chrono::seconds(holdDurationSeconds);
        return true;
    }

    return false;
}

bool SeatManager::confirmSeat(int seatId, const std::string& userId) {
    auto it = seats.find(seatId);
    if (it == seats.end()) return false;

    std::lock_guard<std::mutex> lock(it->second->mtx);
    Seat& seat = it->second->seat;

    auto now = std::chrono::steady_clock::now();

    if (seat.status == SeatStatus::HELD && seat.userId == userId) {
        if (now <= seat.holdExpiry) {
            seat.status = SeatStatus::SOLD;
            return true;
        } else {
            seat.status = SeatStatus::AVAILABLE;
            seat.userId = "";
            return false;
        }
    }

    return false;
}

bool SeatManager::releaseSeat(int seatId) {
    auto it = seats.find(seatId);
    if (it == seats.end()) return false;

    std::lock_guard<std::mutex> lock(it->second->mtx);
    it->second->seat.status = SeatStatus::AVAILABLE;
    it->second->seat.userId = "";
    return true;
}

void SeatManager::cleanupExpiredHolds() {
    auto now = std::chrono::steady_clock::now();
    for (auto& pair : seats) {
        // In a real high-perf system, we might use try_lock or only lock if we think it's expired
        std::lock_guard<std::mutex> lock(pair.second->mtx);
        Seat& seat = pair.second->seat;
        if (seat.status == SeatStatus::HELD && now > seat.holdExpiry) {
            seat.status = SeatStatus::AVAILABLE;
            seat.userId = "";
        }
    }
}

std::vector<SeatManager::SeatInfo> SeatManager::getSeatStates() {
    std::vector<SeatInfo> infos;
    for (int i = 1; i <= (int)seats.size(); ++i) {
        auto it = seats.find(i);
        if (it != seats.end()) {
            std::lock_guard<std::mutex> lock(it->second->mtx);
            infos.push_back({it->second->seat.seatId, it->second->seat.status, it->second->seat.userId});
        }
    }
    return infos;
}
