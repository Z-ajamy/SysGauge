# SysGauge ⏱️

**SysGauge** is a robust, modular Command-Line Interface (CLI) benchmarking tool written in C. It is designed to precisely measure the execution time and memory footprint of multiple programs in a strictly isolated, sequential environment.



## 🎯 The Problem & The Solution
Many benchmarking tools attempt to run programs concurrently, which inevitably leads to system resource contention (CPU, L3 Cache, Memory Controller) and severely skews measurement accuracy. 

**SysGauge** solves this from a systems engineering perspective. It executes each requested program sequentially in a completely isolated child process. This guarantees that each process has full, uninterrupted access to system resources, yielding true, real-world performance metrics (Wall-clock time & Max Resident Set Size).

## ✨ Core Features
* **Total Isolation:** Utilizes standard Unix system calls (`fork`, `execvp`, `wait4`) to spawn and manage independent child processes.
* **High-Precision Metrics:** Leverages `CLOCK_MONOTONIC` to track execution time down to the nanosecond, remaining entirely immune to system time updates (e.g., NTP adjustments).
* **Memory Profiling:** Accurately retrieves the Maximum Resident Set Size (Max RSS) in Kilobytes directly from the kernel.
* **Output Suppression:** Automatically redirects the target programs' STDOUT and STDERR to `/dev/null` to guarantee a clean, readable benchmarking report.
* **Robust Error Handling:** Gracefully detects and reports execution failures (`EXEC_FAILED`), memory access violations (`SEGFAULT`), and unknown terminations.
* **Smart Sorting:** Automatically sorts the final workload array using `qsort`, prioritizing successful executions and ranking them from fastest to slowest.

## 🏗️ Architecture
The codebase is structured around the "Separation of Concerns" principle, ensuring high scalability. It is designed so the core logic can easily be repurposed (e.g., shifting from a CLI tool to an HTTP Server) without rewriting the engine:
1. **Core Engine (`core_engine/`):** The isolated backend responsible for process management, system calls, and metric gathering.
2. **Interface Layer (`interface/`):** Handles CLI input tokenization, dynamic memory allocation, and workload preparation.
3. **Reporting Layer (`reporting/`):** Processes the raw engine output and formats it into a human-readable tabular data structure.

## 🛠️ Prerequisites & Compilation
This project is strictly compliant with the `gnu89` standard and adheres to the **Betty** coding style and documentation guidelines.

Compile the tool in a Linux/Unix environment using `gcc` with strict warning flags to ensure code quality:

```bash
gcc -Wall -pedantic -Werror -Wextra -std=gnu89 main.c core_engine/engine.c interface/interface.c reporting/report.c -o sysgauge
