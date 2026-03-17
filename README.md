# SysGauge ⏱️

SysGauge is a robust, modular Command-Line Interface (CLI) benchmarking tool written in C. It is designed to precisely measure the execution time and memory footprint of multiple programs in a strictly isolated, sequential environment.

---

## 🎯 The Problem & The Solution

Many benchmarking tools attempt to run programs concurrently, which inevitably leads to system resource contention (CPU, L3 Cache, Memory Controller) and severely skews measurement accuracy.

SysGauge solves this from a systems engineering perspective. It executes each requested program sequentially in a completely isolated child process. This guarantees that each process has full, uninterrupted access to system resources, yielding true, real-world performance metrics (Wall-clock time & Max Resident Set Size).

---

## ✨ Core Features

- **Total Isolation:** Utilizes standard Unix system calls (`fork`, `execvp`, `wait4`) to spawn and manage independent child processes.

- **High-Precision Metrics:** Leverages `CLOCK_MONOTONIC` to track execution time down to the nanosecond, remaining entirely immune to system time updates (e.g., NTP adjustments).

- **Memory Profiling:** Accurately retrieves the Maximum Resident Set Size (Max RSS) in Kilobytes directly from the kernel.

- **Output Suppression:** Automatically redirects the target programs' STDOUT and STDERR to `/dev/null` to guarantee a clean, readable benchmarking report.

- **Robust Error Handling:** Gracefully detects and reports execution failures (`EXEC_FAILED`), memory access violations (`SEGFAULT`), and unknown terminations.

- **Smart Sorting:** Automatically sorts the final workload array using `qsort`, prioritizing successful executions and ranking them from fastest to slowest.

---

## 🏗️ Architecture

The codebase is structured around the **"Separation of Concerns"** principle, ensuring high scalability. It is designed so the core logic can easily be repurposed (e.g., shifting from a CLI tool to an HTTP Server) without rewriting the engine:

- **Core Engine (`core_engine/`):** The isolated backend responsible for process management, system calls, and metric gathering.

- **Interface Layer (`interface/`):** Handles CLI input tokenization, dynamic memory allocation, and workload preparation.

- **Reporting Layer (`reporting/`):** Processes the raw engine output and formats it into a human-readable tabular data structure.

---

## 🛠️ Prerequisites & Compilation

This project is strictly compliant with the `gnu89` standard and adheres to the **Betty coding style** and documentation guidelines.

Compile the tool in a Linux/Unix environment using `gcc` with strict warning flags to ensure code quality:

```bash
gcc -Wall -pedantic -Werror -Wextra -std=gnu89 -g \
  main.c core_engine/engine.c interface/interface.c reporting/report.c \
  -o sysgauge
```

> **Note:** The `-g` flag is included to generate debugging symbols for tools like Valgrind.

---

## 🚀 Usage

SysGauge accepts one or more commands as arguments. Each command is tokenized and executed sequentially in its own isolated child process.

**Basic Syntax:**

```bash
./sysgauge "<command_1>" "<command_2>" ... "<command_N>"
```

**Example:**
To benchmark standard Unix commands alongside a custom executable:

```bash
./sysgauge "ls -la" "sleep 2" "./my_custom_program" "pwd"
```

**Expected Output:**

During execution, the standard output (STDOUT) and standard error (STDERR) of the target programs are redirected to `/dev/null` to prevent terminal clutter. Once all benchmarks are complete, SysGauge generates a sorted tabular report displaying:

| Column | Description |
|---|---|
| Command Name | The command that was benchmarked |
| Execution Time | In nanoseconds / milliseconds |
| Memory Footprint | Max RSS in KB |
| Status | `Success`, `EXEC_FAILED`, `SEGFAULT`, etc. |

---

## 🐛 Testing & Debugging (Memory Profiling)

To ensure the tool is free of memory leaks (especially regarding dynamic tokenization and array allocations), use Valgrind.

Ensure you run the executable with `./` so Valgrind can locate it in the current directory:

```bash
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         ./sysgauge "ls -l" "sleep 1"
```
