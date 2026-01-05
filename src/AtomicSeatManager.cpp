#include "AtomicSeatManager.h"
#include <iostream>

/*
 * Implementation Notes:
 * We use Compare-And-Swap (CAS) to perform state transitions.
 * 
 * Logic:
 * 1. Load current state.
 * 2. Check if valid transition is possible.
 * 3. Calculate new state.
 * 4. CAS. If fail, retry (loop).
 * 
 * Note: storing the 'userId' atomically alongside status requires a 128-bit atomic (DWCAS)
 * or packing the bits into a 64-bit integer.
 * For this demo, we assume:
 *  - bit 0-1: Status (00=Avail, 01=Held, 10=Sold)
 *  - bit 2-31: UserId (30 bits)
 *  - bit 32-63: Timestamp/Expiry (32 bits, relative)
 */

AtomicSeatManager::AtomicSeatManager(int totalSeats) {
    seats = std::vector<AtomicSeat>(totalSeats + 1);
    for (auto& seat : seats) {
        seat.state = 0; // AVAILABLE, No User
    }
}

bool AtomicSeatManager::holdSeat(int seatId, int userId, int durationMs) {
    if (seatId >= seats.size()) return false;
    AtomicSeat& seat = seats[seatId];

    while (true) {
        int current = seat.state.load(std::memory_order_acquire);
        
        // Unpack status (first 2 bits)
        // 0 = AVAILABLE
        if ((current & 3) != 0) {
            // Check if it's expired HELD? 
            // In a pure packed integer model, we'd check the timestamp bits.
            // For simplicity here, if it's not 0, we assume taken.
            return false;
        }

        // Construct new state: Status=1 (HELD), User=userId
        // We are ignoring timestamp packing for brevity in this snippet, 
        // but in production you'd OR it in.
        int desired = (userId << 2) | 1;

        if (seat.state.compare_exchange_weak(current, desired, std::memory_order_release, std::memory_order_relaxed)) {
            // Success!
            return true;
        }
        // If CAS failed, 'current' is updated with the real current value, loop and check again.
    }
}

bool AtomicSeatManager::confirmSeat(int seatId, int userId) {
    if (seatId >= seats.size()) return false;
    AtomicSeat& seat = seats[seatId];

    while (true) {
        int current = seat.state.load(std::memory_order_acquire);

        // Must be HELD (1)
        if ((current & 3) != 1) return false;

        // Must be held by THIS user
        int holder = current >> 2;
        if (holder != userId) return false;

        // Transition to SOLD (2), keep user
        int desired = (userId << 2) | 2;

        if (seat.state.compare_exchange_weak(current, desired, std::memory_order_release, std::memory_order_relaxed)) {
            return true;
        }
    }
}

bool AtomicSeatManager::releaseSeat(int seatId) {
    if (seatId >= seats.size()) return false;
    AtomicSeat& seat = seats[seatId];
    
    // Blind reset to AVAILABLE (0), or use CAS if we want to be safe against double-frees?
    // A simple store is "Lock-Free" but might overwrite a NEW hold if we are valid.
    // So we must use CAS loop to ensure we only release if it is not SOLD.
    
    while (true) {
        int current = seat.state.load(std::memory_order_acquire);
        int status = current & 3;
        
        if (status == 2) { // SOLD - can't release normally?
             // Depends on biz logic. Let's say NO.
             return false;
        }

        // Set to AVAILABLE (0)
        int desired = 0;
        if (seat.state.compare_exchange_weak(current, desired, std::memory_order_release, std::memory_order_relaxed)) {
            return true;
        }
    }
}

std::vector<AtomicSeatManager::SeatSnapshot> AtomicSeatManager::getSnapshot() {
    std::vector<SeatSnapshot> snap;
    snap.reserve(seats.size());
    for(size_t i=1; i<seats.size(); ++i) {
        int val = seats[i].state.load(std::memory_order_relaxed);
        int statusInt = val & 3;
        int uId = val >> 2;
        snap.push_back({(int)i, (AtomicStatus)statusInt, uId});
    }
    return snap;
}
