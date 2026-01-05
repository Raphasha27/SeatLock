const API_BASE = 'http://localhost:8000';
const WS_BASE = 'ws://localhost:8000';
let currentUserId = 1;
let seats = [];
let ws = null;

// Initialize
document.addEventListener('DOMContentLoaded', () => {
    currentUserId = parseInt(document.getElementById('user-id').value);
    document.getElementById('user-id').addEventListener('change', (e) => {
        currentUserId = parseInt(e.target.value);
        renderSeats();
    });
    
    // Connect to WebSocket
    connectWebSocket();
    
    loadSeats();
    // Auto-refresh every 3 seconds as fallback
    setInterval(loadSeats, 3000);
});

function connectWebSocket() {
    ws = new WebSocket(`${WS_BASE}/ws`);
    
    ws.onopen = () => {
        console.log('WebSocket connected');
        showToast('Connected to SeatLock Engine', 'success');
    };
    
    ws.onmessage = (event) => {
        const data = JSON.parse(event.data);
        if (data.type === 'seat_update') {
            // Real-time update from server
            console.log('Seat update received:', data);
            loadSeats(); // Refresh to show changes
        }
    };
    
    ws.onerror = (error) => {
        console.error('WebSocket error:', error);
    };
    
    ws.onclose = () => {
        console.log('WebSocket disconnected, reconnecting...');
        setTimeout(connectWebSocket, 3000);
    };
}

async function loadSeats() {
    try {
        const response = await fetch(`${API_BASE}/seats`);
        if (!response.ok) throw new Error('Failed to fetch seats');
        
        seats = await response.json();
        renderSeats();
        updateStats();
    } catch (error) {
        console.error('Error loading seats:', error);
        showToast('Failed to load seats', 'error');
    }
}

function renderSeats() {
    const grid = document.getElementById('seats-grid');
    grid.innerHTML = '';
    
    // Create 50 seats for demo
    for (let i = 1; i <= 50; i++) {
        const seat = document.createElement('div');
        seat.className = 'seat';
        seat.textContent = i;
        
        // Simulate seat status (in real app, this comes from backend)
        const random = Math.random();
        if (random < 0.7) {
            seat.classList.add('available');
            seat.onclick = () => holdSeat(i);
        } else if (random < 0.85) {
            seat.classList.add('held');
            seat.onclick = () => confirmSeat(i);
        } else {
            seat.classList.add('sold');
        }
        
        grid.appendChild(seat);
    }
}

async function holdSeat(seatId) {
    try {
        const response = await fetch(`${API_BASE}/hold`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                seat_id: seatId,
                user_id: currentUserId
            })
        });
        
        if (!response.ok) {
            const error = await response.json();
            throw new Error(error.detail || 'Failed to hold seat');
        }
        
        const data = await response.json();
        showToast(`Seat ${seatId} held successfully! Click again to confirm.`, 'success');
        await loadSeats();
    } catch (error) {
        console.error('Error holding seat:', error);
        showToast(error.message, 'error');
    }
}

async function confirmSeat(seatId) {
    try {
        const response = await fetch(`${API_BASE}/confirm`, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json',
            },
            body: JSON.stringify({
                seat_id: seatId,
                user_id: currentUserId
            })
        });
        
        if (!response.ok) {
            const error = await response.json();
            throw new Error(error.detail || 'Failed to confirm seat');
        }
        
        const data = await response.json();
        showToast(`Seat ${seatId} confirmed! Payment successful.`, 'success');
        await loadSeats();
    } catch (error) {
        console.error('Error confirming seat:', error);
        showToast(error.message, 'error');
    }
}

function updateStats() {
    // Count seats by status
    const seatElements = document.querySelectorAll('.seat');
    let available = 0, held = 0, sold = 0;
    
    seatElements.forEach(seat => {
        if (seat.classList.contains('available')) available++;
        else if (seat.classList.contains('held')) held++;
        else if (seat.classList.contains('sold')) sold++;
    });
    
    // Animate counter
    animateValue('available-count', available);
    animateValue('held-count', held);
    animateValue('sold-count', sold);
}

function animateValue(id, value) {
    const element = document.getElementById(id);
    const current = parseInt(element.textContent) || 0;
    
    if (current === value) return;
    
    const duration = 500;
    const step = (value - current) / (duration / 16);
    let currentValue = current;
    
    const timer = setInterval(() => {
        currentValue += step;
        if ((step > 0 && currentValue >= value) || (step < 0 && currentValue <= value)) {
            element.textContent = value;
            clearInterval(timer);
        } else {
            element.textContent = Math.round(currentValue);
        }
    }, 16);
}

function showToast(message, type = 'success') {
    const toast = document.getElementById('toast');
    toast.textContent = message;
    toast.className = `toast show ${type}`;
    
    setTimeout(() => {
        toast.classList.remove('show');
    }, 3000);
}
