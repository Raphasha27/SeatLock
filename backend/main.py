from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
# import grpc
# import seatlock_pb2
# import seatlock_pb2_grpc

app = FastAPI(title="SeatLock Wrapper")

# Mocking gRPC Client for now since we can't compile the proto in this environment
class MockGrpcClient:
    def hold(self, seat_id, user_id):
        return True
    def confirm(self, seat_id, user_id):
        return True
    def get_map(self):
        return [{"seat_id": 1, "status": 0, "user_id": 0}]

grpc_client = MockGrpcClient()

class HoldReq(BaseModel):
    seat_id: int
    user_id: int

@app.post("/hold")
def hold_seat(req: HoldReq):
    success = grpc_client.hold(req.seat_id, req.user_id)
    if not success:
        raise HTTPException(status_code=409, detail="Seat taken")
    return {"status": "held", "seat_id": req.seat_id}

@app.post("/confirm")
def confirm_seat(req: HoldReq):
    success = grpc_client.confirm(req.seat_id, req.user_id)
    if not success:
        raise HTTPException(status_code=400, detail="Cannot confirm")
    return {"status": "sold", "seat_id": req.seat_id}

@app.get("/seats")
def get_seats():
    return grpc_client.get_map()
