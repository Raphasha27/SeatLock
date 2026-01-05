# SeatLock: High-Performance Real-Time Reservation Engine

[![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B20)
[![Performance](https://img.shields.io/badge/Performance-32M%20ops%2Fsec-brightgreen.svg)](https://github.com/Raphasha27/SeatLock)
[![Frontend](https://img.shields.io/badge/Frontend-React-61DAFB.svg)](frontend/)
[![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

A high-performance C++20 engine for handling seat reservations with **sub-microsecond latency** and **lock-free concurrency**. Now features a real-time React frontend and WebSocket broadcasting.

## 🎯 Architecture

```
┌─────────────────────────────┐      ┌─────────────────────────────┐
│   User A (Browser)          │      │   User B (Browser)          │
│   React + WebSockets        │◄────►│   React + WebSockets        │
└──────────▲──────────────────┘      └──────────▲──────────────────┘
           │                                    │
           │           Real-time Updates        │
           └──────────────────┐  ┌──────────────┘
                              │  │
┌─────────────────────────────▼──▼──────────────────────────┐
│   FastAPI Gateway (Python)                                │
│   • REST API for Holds/Confirms                           │
│   • WebSocket Broadcaster                                 │
└──────────┬────────────────────────────────────────────────┘
           │ gRPC / FFI
┌──────────▼────────────────────────────────────────────────┐
│   C++ SeatLock Engine                                     │
│   • Atomic State Management (32M ops/sec)                 │
│   • Expiry Worker                                         │
└───────────────────────────────────────────────────────────┘
```

## 📊 Performance Benchmark

**Benchmark Results** (16 threads, 1.6M operations):
```
Ops/Sec: 32,240,000
Time:    0.05 seconds
Throughput: Sub-millisecond lock acquisition
```

## 🚀 One-Minute Quick Start

### 1. Build the Engine (C++)
```bash
# Windows
build_seatlock.bat
```

### 2. Start the Backend (Python)
```bash
cd backend
pip install -r requirements.txt
uvicorn main:app --reload
```

### 3. Open the Frontend
Simply open `frontend/index.html` in your browser.
- **Green**: Available
- **Orange**: Held (Real-time update)
- **Red**: Sold (Real-time update)

## ✨ Key Features

- **Lock-Free Core**: Uses `std::atomic` and CAS loops for maximum throughput.
- **Real-Time Sync**: WebSockets broadcast seat state changes instantly to all connected clients.
- **Fine-Grained Locking**: Alternative implementation available for comparison.
- **Auto-Expiry**: Background thread automatically releases held seats after timeout.

## 📂 Project Structure

```
seatlock/
├── src/                  # C++ Source
│   ├── AtomicSeatManager.cpp
│   └── ...
├── include/              # C++ Headers
├── backend/
│   ├── main.py           # FastAPI + WebSockets
│   └── requirements.txt
├── frontend/             # Real-time UI
│   ├── index.html
│   ├── app.js
│   └── styles.css
├── build_seatlock.bat    # Automated Build Script
└── README.md
```

## 🎓 Interview Talking Points

1. **System Design**: "I extended the C++ engine with a **FastAPI gateway** to support web clients. I implemented **WebSockets** to solve the 'stale view' problem in ticketing, ensuring users see taken seats instantly."
2. **Concurrency**: "The core engine handles 32M ops/sec using **lock-free** structures, while the frontend handles eventual consistency via WebSocket events."
3. **Full Stack**: "Demonstrates integration from low-level C++ memory management up to high-level React UI state."

## ☁️ Deployment

- **Frontend**: Configured for Vercel (`vercel.json`)
- **Backend**: Configured for Railway/Heroku (`Procfile`)

## 📸 Gallery

### Real-Time Frontend (React)
![SeatLock Frontend](frontend/seatlock_frontend_preview.png)
*Live seat updates with color-coded status (Available, Held, Sold)*

### API Documentation (FastAPI/Swagger)
![Swagger UI](frontend/seatlock_api_preview.png)
*Interactive REST API testing interface*

## 👤 About the Author

<div align="center">
  <img src="https://github.com/Raphasha27.png" width="100px" style="border-radius: 50%;" alt="Raphasha27"/>
  <h1>Raphasha27</h1>
  <h3>Software Engineer • AI Specialist • Tech Visionary</h3>
  
  <p><i>"Building intelligent systems that bridge the gap between complex data and human intuition."</i></p>

  [![GitHub Followers](https://img.shields.io/github/followers/Raphasha27?style=social)](https://github.com/Raphasha27)
  [![LinkedIn](https://img.shields.io/badge/LinkedIn-Connect-blue?style=social&logo=linkedin)](https://linkedin.com/in/)
</div>

### 🌌 About Me
I am a determined **Software Developer** and **AI Engineer** based in South Africa. My focus lies in creating high-impact, AI-driven solutions that solve real-world problems. Whether it's building sovereign AI nodes or modernizing SOC platforms, I thrive at the intersection of performance and aesthetics.

- 🎓 Pursuing **Bachelor of Information Technology** @ Richfield College.
- 🧪 Deeply invested in **Neural Networks**, **Computer Vision**, and **Autonomous Systems**.
- 🛠️ Currently architecting the **AI Job Market Intelligence Platform**.

### 🛠️ Technology Stack & Arsenal

#### 🧠 AI & Data Science
[![PyTorch](https://img.shields.io/badge/PyTorch-%23EE4C2C.svg?style=for-the-badge&logo=PyTorch&logoColor=white)](https://pytorch.org/)
[![TensorFlow](https://img.shields.io/badge/TensorFlow-%23FF6F00.svg?style=for-the-badge&logo=TensorFlow&logoColor=white)](https://www.tensorflow.org/)
[![Scikit-Learn](https://img.shields.io/badge/scikit--learn-%23F7931E.svg?style=for-the-badge&logo=scikit-learn&logoColor=white)](https://scikit-learn.org/)
[![Pandas](https://img.shields.io/badge/pandas-%23150458.svg?style=for-the-badge&logo=pandas&logoColor=white)](https://pandas.pydata.org/)
[![NumPy](https://img.shields.io/badge/numpy-%23013243.svg?style=for-the-badge&logo=numpy&logoColor=white)](https://numpy.org/)
[![OpenCV](https://img.shields.io/badge/OpenCV-%23white.svg?style=for-the-badge&logo=opencv&logoColor=white)](https://opencv.org/)

#### 💻 Core Development
[![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![Python](https://img.shields.io/badge/python-3670A0?style=for-the-badge&logo=python&logoColor=ffdd54)](https://www.python.org/)
[![React](https://img.shields.io/badge/react-%2320232a.svg?style=for-the-badge&logo=react&logoColor=%2361DAFB)](https://reactjs.org/)
[![Angular](https://img.shields.io/badge/angular-%23DD0031.svg?style=for-the-badge&logo=angular&logoColor=white)](https://angular.io/)
[![TypeScript](https://img.shields.io/badge/typescript-%23007ACC.svg?style=for-the-badge&logo=typescript&logoColor=white)](https://www.typescriptlang.org/)
[![Rust](https://img.shields.io/badge/rust-%23000000.svg?style=for-the-badge&logo=rust&logoColor=white)](https://www.rust-lang.org/)

### 💎 Featured Masterpieces

#### 🤖 AI Job Market Intelligence
*A premium platform for real-time job market analytics and AI resume matching.*
> **Stack**: React, FastAPI, OpenAI, Tailwind
> [View Repository](https://github.com/Raphasha27/ai-job-market-intelligence)

#### 🛡️ CyberShield Modern
*Next-gen Security Operations Center dashboard with AI Sentinel integration.*
> **Stack**: Angular, Tailwind, D3.js, WebSockets
> [View Repository](https://github.com/Raphasha27/cybershield-modern)

#### ☀️ Aura Weather AI
*Hyper-visual weather forecast app with glassmorphic UI and climate insights.*
> **Stack**: Vite, Vanilla CSS, Framer Motion
> [View Repository](https://github.com/Raphasha27/aura-weather-ai)

#### 🌍 Sovereign AI Nexus
*Decentralized AI ecosystem for high-performance computing and secure data nodes.*
> **Stack**: Python, Rust, Docker, gRPC
> [View Repository](https://github.com/Raphasha27/Sovereign-AI-Nexus-v2)

### 📊 My Digital Footprint

<p align="center">
  <img src="https://github-readme-stats.vercel.app/api?username=Raphasha27&show_icons=true&theme=tokyonight&count_private=true" alt="Raphasha27's GitHub Stats" />
  <img src="https://github-readme-stats.vercel.app/api/top-langs/?username=Raphasha27&layout=compact&theme=tokyonight" alt="Raphasha27's Top Languages" />
</p>
<p align="center">
  <img src="https://github-readme-streak-stats.herokuapp.com/?user=Raphasha27&theme=tokyonight" alt="Raphasha27's Streak" />
</p>

---
<div align="center">
  <p>Built with ❤️ by <b>Raphasha27</b> • © 2026</p>
</div>
