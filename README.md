# SeatLock

Real-time seat reservation system with lock-based and lock-free concurrency implementations in C++.

## Tech Stack

- **Language:** C++20
- **Build:** CMake
- **Concurrency:** Mutex-based (pthreads) and lock-free (atomics)
- **Python:** Tooling scripts

## Features

- Dual implementation: mutex-based and lock-free seat managers
- Automatic seat expiry with background worker thread
- TCP server interface for remote reservation
- Performance benchmarking suite

## Getting Started

```bash
git clone https://github.com/Raphasha27/SeatLock.git
cd SeatLock
mkdir build && cd build
cmake ..
cmake --build .
./SeatLock
```

## License

MIT
