#pragma once
#include <vector>
#include <atomic>
#include <string>
#include <memory>
#include <chrono>

// PACKING STATE:
// To be truly lock-free, we often stick to primitive types.
// We'll use a 64-bit atomic to store: { Status (2 bits) | UserIDHash (30 bits) | Timestamp (32 bits) }
// This allows atomic distinct operations.
// OR we can use std::atomic<SeatEntry*> with RCU-like behavior (advanced).
// For this interview-level showcase, we will use atomic state transitions + spinlocks for data.
// This is effectively "Lock-Free State, Lock-Based Data" which is very fast.

enum class AtomicStatus : int {
    AVAILABLE = 0,
    HELD = 1,
    SOLD = 2
};

struct AtomicSeat {
    std::atomic<int> state; // Encodes status
    int userId = 0;         // Simple numeric ID for lock-free simplicity (hashing string)
    long long holdExpiry = 0; // Epoch ms
};

class AtomicSeatManager {
public:
    AtomicSeatManager(int totalSeats);

    // Returns true if successfully moved from AVAILABLE -> HELD
    bool holdSeat(int seatId, int userId, int durationMs);

    // Returns true if successfully moved from HELD -> SOLD
    bool confirmSeat(int seatId, int userId);

    // Returns true if released
    bool releaseSeat(int seatId);

    struct SeatSnapshot {
        int id;
        AtomicStatus status;
        int userId;
    };
    std::vector<SeatSnapshot> getSnapshot();

private:
    std::vector<AtomicSeat> seats;
};
