#include "Server.h"
#include <iostream>

Server::Server(SeatManager& manager, int p) : seatManager(manager), port(p) {}

void Server::run() {
    std::cout << "Server starting on port " << port << " (Not implemented yet - requires gRPC/Beast)..." << std::endl;
    // Here we would set up the gRPC service or HTTP listener
    // and map endpoints to seatManager methods:
    // e.g. POST /hold -> seatManager.holdSeat(...)
}
