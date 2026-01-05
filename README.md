# Socket Money 💸

A fast, CLI-based financial ledger built in **C** and **SQLite**.

SocketMoney is a lightweight alternative to subscription-based finance apps (like RocketMoney), designed to run locally with a focus on data privacy, speed, and strict memory management.

# Features

* **Persistent Storage:** Uses an embedded SQLite database (`socketmoney.db`) to store transaction history.
* **CRUD Operations:** Create and Read financial data directly from the command line.
* **Financial Analytics:** Aggregates data to calculate total lifetime spending using SQL aggregate functions.
* **Input Hygiene:** Robust handling of user input to prevent buffer overflows and infinite loops.

## Architecture & Design Patterns
This project demonstrates several core systems programming concepts relevant to financial technology:

### 1. SQL Injection Prevention
Instead of using `sprintf` to concatenate SQL strings (which is vulnerable to injection), this project uses **SQLite Prepared Statements** (`sqlite3_prepare_v2`). 
* This ensures user input is treated strictly as data, not executable code, preventing malicious SQL injection attacks.

### 2. Modular Compilation
The codebase is refactored into **Interface (`storage.h`)**, **Implementation (`storage.c`)**, and **Application Logic (`main.c`)**.
* This separation of concerns improves maintainability and scalability, allowing the database logic to be swapped or updated without breaking the main application flow.

### 3. Memory Safety
* **Buffer Limits:** `scanf` widths (e.g., `%49s`) are explicitly set to prevent buffer overflow vulnerabilities.
* **Resource Management:** All database connections and statements are explicitly finalized and closed to prevent memory leaks.

## Build Instructions

### Prerequisites
You need a C compiler (`gcc`) and the SQLite development libraries.
* **Debian/Ubuntu:** `sudo apt-get install libsqlite3-dev`
* **MacOS:** `brew install sqlite3` (or pre-installed)

### Compilation
Compile the modular files and link the SQLite library:

```bash
gcc main.c storage.c -o socketMoney -lsqlite3