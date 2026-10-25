<h1 align="center">Cloud TAP — Technical Documentation</h1>

<p align="center">
  <img src="https://img.shields.io/badge/LANGUAGE-C%2B%2B17-0d1117?style=for-the-badge&labelColor=0d1117&color=00b4d8" alt="Language"/>
  <img src="https://img.shields.io/badge/DEVICES-48-0d1117?style=for-the-badge&labelColor=0d1117&color=00b4d8" alt="Devices"/>
  <img src="https://img.shields.io/badge/REPORTS-2-0d1117?style=for-the-badge&labelColor=0d1117&color=00b4d8" alt="Reports"/>
</p>

<p align="center">
  <a href="#contents"><b>Contents</b></a> ·
  <a href="#architecture"><b>Architecture</b></a> ·
  <a href="#data-structures"><b>Data Structures</b></a> ·
  <a href="#algorithms"><b>Algorithms</b></a> ·
  <a href="#testing"><b>Testing</b></a> ·
  <a href="#build--run"><b>Build</b></a>
</p>

---

This folder holds the source code and the two project reports. For the overview, topology and screenshots, see the [main README](../README.md).

## Contents

| File | Description |
| ---- | ----------- |
| [`cloud.cpp`](./cloud.cpp) | Source code of the full 48-device simulator |
| [`Cloud-TAP-Full.pdf`](./Cloud-TAP-Full.pdf) | Full report (27 pages): architecture, protocols, complexity analysis, testing |
| [`Cloud-TAP-Mini.pdf`](./Cloud-TAP-Mini.pdf) | Mini report (16 pages): 9-device prototype with custom data structures |

## Architecture

The simulator is layered. Each layer only talks to the one below it.

| Layer | Responsibility |
| ----- | -------------- |
| **Interface** | Console control panel (10 options) and export to the web dashboard |
| **Services** | DHCP, NAT, firewall ACL, HSRP, OSPF neighbours, Syslog |
| **Algorithms** | BFS, DFS, impact analysis, ACL matching |
| **Core model** | Device registry and connection graph |

## Data Structures

| Structure | Role | Complexity |
| --------- | ---- | ---------- |
| `map<string, Device>` | Device registry, sorted iteration | `O(log n)` |
| `vector<Connection>` | Adjacency list per device | `O(degree)` |
| `set<int>` | Used addresses per DHCP pool | `O(log m)` |
| `deque<SyslogEntry>` | Circular log buffer, 1,000 entries | `O(1)` |
| `queue<string>` | BFS frontier | `O(1)` |
| `set<string>` | Visited nodes for DFS | `O(log n)` |

The graph is sparse (75 links among 48 devices), so an adjacency list uses far less memory than a 48 × 48 matrix.

The Mini report implements a binary search tree, graph, queue, stack and priority queue from scratch to show how each one works internally.

## Algorithms

| Algorithm | Purpose | Complexity |
| --------- | ------- | ---------- |
| BFS | Fewest-hop path between two devices | `O(V + E)` |
| DFS | Cascading failure and impact analysis | `O(V + E)` |
| Dijkstra (Mini) | Minimum-latency path | `O((V + E) log V)` |
| ACL matching | First-match firewall decision, CIDR-aware | `O(R)` |
| NAT translation | Private-to-public mapping with timeout | `O(n)` |

OSPF is modelled at the neighbour level (hello and dead timers). Route selection uses static configuration, not a full Dijkstra-based SPF calculation.

## Testing

| Category | What was checked |
| -------- | ---------------- |
| Normal operation | Lookup, path finding, DHCP allocation, logging |
| Edge cases | Unreachable targets, empty pools, duplicate IDs |
| Error handling | Invalid input, removed devices, full log buffer |
| Dynamic events | Device removal and failover while the system is running |
| Memory | Checked with valgrind, no leaks found |

## Build & Run

```bash
g++ -std=c++17 -O2 -o cloud-tap cloud.cpp
./cloud-tap
```

Requires a C++17 compiler (g++ or clang++). No external libraries.

## Authors

| Member | Contribution |
| ------ | ------------ |
| **Abdul Wahid** | Network topology design, binary search tree, connection stack, system integration, testing |
| **Uneeb Ali** | DHCP queue, graph and traversal algorithms, priority-queue event logger, complexity analysis |
