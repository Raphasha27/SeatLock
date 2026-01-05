#include <iostream>
#include <cassert>
#include "SeatManager.h"

int main() {
    SeatManager manager(10);

    // Test 1: Hold a seat
    bool held = manager.holdSeat(1, "User1", 10);
    assert(held == true);
    std::cout << "Test 1 Passed: Hold seat\n";

    // Test 2: Double hold should fail
    bool held2 = manager.holdSeat(1, "User2", 10);
    assert(held2 == false);
    std::cout << "Test 2 Passed: Double hold fail\n";

    // Test 3: Confirm seat
    bool confirmed = manager.confirmSeat(1, "User1");
    assert(confirmed == true);
    std::cout << "Test 3 Passed: Confirm seat\n";

    // Test 4: Release seat (if sold, confirm logic might keep it sold or separate logic needed? 
    // In our logic, SOLD is final unless we have a refund/admin release. 
    // releaseSeat forces AVAILABLE.)
    manager.releaseSeat(1);
    auto states = manager.getSeatStates();
    assert(states[0].status == SeatStatus::AVAILABLE);
    std::cout << "Test 4 Passed: Release seat\n";

    std::cout << "All basic tests passed.\n";
    return 0;
}
