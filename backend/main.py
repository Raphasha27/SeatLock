from fastapi import FastAPI, HTTPException, WebSocket, WebSocketDisconnect
from fastapi.middleware.cors import CORSMiddleware
from pydantic import BaseModel
from typing import List
import asyncio
import json
# import grpc
# import seatlock_pb2
# import seatlock_pb2_grpc

app = FastAPI(title="SeatLock Wrapper")

# Enable CORS
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # In production, specify exact origins
    allow_credentials=True,
    allow_methods=["*"],
    allow_headers=["*"],
)

# WebSocket connection manager
class ConnectionManager:
    def __init__(self):
        self.active_connections: List[WebSocket] = []

    async def connect(self, websocket: WebSocket):
        await websocket.accept()
        self.active_connections.append(websocket)

    def disconnect(self, websocket: WebSocket):
        self.active_connections.remove(websocket)

    async def broadcast(self, message: dict):
        for connection in self.active_connections:
            try:
                await connection.send_json(message)
            except:
                pass

manager = ConnectionManager()

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
async def hold_seat(req: HoldReq):
    success = grpc_client.hold(req.seat_id, req.user_id)
    if not success:
        raise HTTPException(status_code=409, detail="Seat taken")
    
    # Broadcast to all connected clients
    await manager.broadcast({
        "type": "seat_update",
        "seat_id": req.seat_id,
        "status": "held",
        "user_id": req.user_id
    })
    
    return {"status": "held", "seat_id": req.seat_id}

@app.post("/confirm")
async def confirm_seat(req: HoldReq):
    success = grpc_client.confirm(req.seat_id, req.user_id)
    if not success:
        raise HTTPException(status_code=400, detail="Cannot confirm")
    
    # Broadcast to all connected clients
    await manager.broadcast({
        "type": "seat_update",
        "seat_id": req.seat_id,
        "status": "sold",
        "user_id": req.user_id
    })
    
    return {"status": "sold", "seat_id": req.seat_id}

@app.get("/seats")
def get_seats():
    return grpc_client.get_map()

@app.websocket("/ws")
async def websocket_endpoint(websocket: WebSocket):
    await manager.connect(websocket)
    try:
        while True:
            # Keep connection alive and receive messages
            data = await websocket.receive_text()
            # Echo back or handle client messages
            await websocket.send_json({"type": "pong"})
    except WebSocketDisconnect:
        manager.disconnect(websocket)
