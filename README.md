# Event-Driven HTTP Server

A from-scratch, educational implementation of the HTTP/1.1 protocol using C, raw TCP sockets, and the Reactor pattern.

---

## Table of Contents

- [Features](https://www.google.com/search?q=%23-features)
- [Learning Objectives](https://www.google.com/search?q=%23-learning-objectives)
- [Architecture Deep Dive](https://www.google.com/search?q=%23-architecture-deep-dive)
- [Directory Structure](https://www.google.com/search?q=%23-directory-structure)
- [Building & Running](https://www.google.com/search?q=%23-building--running)
- [Configuration](https://www.google.com/search?q=%23-configuration)
- [Key Concepts Explained](https://www.google.com/search?q=%23-key-concepts-explained)
- [Roadmap / Status](https://www.google.com/search?q=%23-roadmap--status)
- [Contributing & License](https://www.google.com/search?q=%23-contributing--license)

---

## Features

- **Raw Socket Implementation:** Manages the full lifecycle of TCP connections without high-level networking abstractions.
- **Non-Blocking I/O:** Utilizes asynchronous system calls to prevent the server from stalling on slow network operations.
- **Reactor Pattern:** An event-driven architecture that de-multiplexes events and dispatches them to specific handlers.
- **Memory Efficient:** Fixed-size buffering per connection to prevent heap fragmentation and control memory footprint.
- **Scalable Multiplexing:** Leverages Linux `epoll` to monitor thousands of concurrent connections efficiently.

---

## Learning Objectives

This project serves as a practical exploration of systems programming and network architecture. It is designed to demonstrate:

1. **I/O Multiplexing:** How to handle multiple clients on a single thread using `epoll_wait`.
2. **State Machine Design:** Managing the transition between reading headers, processing bodies, and writing responses in a non-blocking environment.
3. **Protocol Parsing:** Implementing the HTTP/1.1 specification, including header parsing and status line generation.
4. **Concurrency Models:** Moving from a single-threaded event loop to a multi-threaded worker pool model.

---

## Architecture Deep Dive

Aegis is built on a modular, layered architecture designed to separate low-level networking concerns from high-level protocol logic.

### Module Responsibilities

| Module             | Responsibility                                                                                                                    |
| ------------------ | --------------------------------------------------------------------------------------------------------------------------------- |
| **`/net`**         | Low-level socket creation, `setsockopt` configuration, and robust wrappers for partial `read`/`write` operations.                 |
| **`/connections`** | Lifecycle management. Maintains a global `uthash` map of active connection states and interacts with the `epoll` file descriptor. |
| **`/events`**      | The "Engine." Implements the core Reactor loop, polling for events and dispatching tasks to connection handlers.                  |
| **`/http`**        | (In Progress) The protocol parser. Decodes byte streams into Request structures and serializes Response objects.                  |
| **`/workers`**     | (In Progress) The thread pool. Offloads CPU-intensive tasks (parsing/routing) from the main event loop to background workers.     |

---

## 📁 Directory Structure

```text
.
├── src
│   ├── net           # Socket abstraction and listener logic
│   ├── connections   # Connection tracking (uthash) and lifecycle
│   ├── events        # Epoll-based Reactor loop
│   ├── http          # [WIP] HTTP parser and state machine
│   ├── workers       # [WIP] Thread pool implementation
│   └── routing       # [WIP] Path-to-handler mapping
├── include           # Header files
├── external          # Third party
└── Makefile          # Build system configuration

```

---

## 🚀 Building & Running

### Prerequisites

- Linux (required for `epoll` support)
- GCC or Clang
- `make`
- `uthash` library (header-only)

### Compilation

```bash
# Clone the repository
git clone https://github.com/DreamInBits01/c-client-server.git
cd c-client-server

# Build the project
make

```

### Running the Server

```bash
# Start the server on a specific port (for now hardcoded on 5050)
make run

```

---

## Configuration



| Constant       | Value | Description                                      |
| -------------- | ----- | ------------------------------------------------ |
| `MAX_EVENTS`   | 512   | Maximum events processed per `epoll_wait` cycle. |
| `REQ_BUF_SIZE` | 8192  | 8 KB buffer for incoming request data.           |
| `RES_BUF_SIZE` | 16384 | 16 KB buffer for outgoing response data.         |

---

## Key Concepts Explained

### The Reactor Pattern

The Reactor pattern is an event-handling pattern for handling service requests delivered concurrently to a service provider by one or more inputs. The service provider then demultiplexes the incoming requests and dispatches them synchronously to the associated request handlers.

### Non-Blocking I/O

In Aegis, sockets are set to `O_NONBLOCK`. This means `read` and `write` calls return immediately. If data isn't ready, the thread isn't "parked"; instead, it returns to the event loop to check other connections, maximizing throughput.

### Epoll 

Unlike `select` or `poll`, `epoll` is an operation regarding the number of monitored file descriptors. It scales efficiently to tens of thousands of connections, making it the standard for high-performance Linux networking.

---

## Roadmap / Status

- [x] **Phase 1: Networking Core** (Socket abstraction, Listener logic)
- [x] **Phase 2: Event Loop** (Epoll integration, Reactor pattern)
- [x] **Phase 3: Connection Management** (Uthash integration, state tracking)
- [ ] **Phase 4: HTTP Parsing** (State machine for header/body parsing) - **UNDER CONSTRUCTION**
- [ ] **Phase 5: Thread Pooling** (Offloading work to worker threads) - **UNDER CONSTRUCTION**
- [ ] **Phase 6: Routing Engine** (Static and dynamic route handlers) - **PLANNED**

---

## License

**License:** MIT License. See `LICENSE` for details.

---
