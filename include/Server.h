#pragma once
#include "SeatManager.h"

// Placeholder for a gRPC or REST server
class Server {
public:
    Server(SeatManager& manager, int port);
    void run();

private:
    SeatManager& seatManager;
    int port;
};
