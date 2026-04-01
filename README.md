# 💳 Bank Management System (C)

*A console-based banking system built in C demonstrating file handling, structured data management, and core systems programming concepts.*

---

## 📌 Overview

This project is a fully functional **Bank Management System** implemented in C. It simulates real-world banking operations using **binary file storage** for persistent data management.

The system allows users to create accounts, authenticate, and perform essential banking operations such as deposits, withdrawals, and account management through a clean, color-coded, menu-driven interface.

---

## ⚙️ Core Concepts Demonstrated

- **Structures (`struct`):** For robust user data modeling.
- **Pointers:** For efficient data manipulation and passing references.
- **Binary File Handling:** Utilizing `fread`, `fwrite`, and `fseek` for persistent storage.
- **In-place Record Updates:** Modifying specific user balances directly in the file using file positioning (`SEEK_CUR`, `SEEK_SET`).
- **Modular Design:** Function-based architecture separating authentication, transactions, and UI.
- **String Handling:** Utilizing `<string.h>` functions like `strcmp` and `strcpy`.
- **Basic Authentication:** Account number and password verification loop.

---

## 🚀 Features

### 👤 User Management
- **Register New Accounts:** Generates a new user profile with a starting balance of 0.00.
- **Secure Login:** Authenticates using the generated account number and password.
- **Delete Account:** Completely removes user records from the system.

### 💰 Banking Operations
- **Deposit Funds:** Safely adds to the current balance.
- **Withdraw Funds:** Subtracts from the balance (includes insufficient funds validation).
- **Balance Inquiry:** Displays current account standing and details.

### ⚙️ Account Settings
- **Change Password:** Updates the user's credentials in real-time.

---

## 🧠 Implementation Highlights

- **Persistent Storage:** Uses a binary file (`Password_records.txt`) to ensure data remains intact between program executions.
- **Direct Modification:** Implements `fseek()` to overwrite existing data blocks without rewriting the entire file, ensuring efficiency during deposits and withdrawals.
- **Safe Deletion:** Ensures data consistency by migrating active accounts to a temporary file (`Temp_records.txt`), deleting the original file, and renaming the temp file.
- **Continuous Interaction:** Loop-based menu system (`while(1)`) keeps the application running until the user explicitly exits.

---

## 🖥️ Interface

- **Command Line Interface (CLI):** Fully menu-driven for ease of use.
- **ANSI Color Coding:** Utilizes terminal escape codes (Red, Green, Turquoise, Orange) to distinguish titles, inputs, errors, and success messages.
- **Structured Navigation:**
  - Main Menu (Login / Register / Exit)
  - Submenu (Banking Operations)
  - Account Settings (Security / Deletion)

---

## ⚠️ Limitations & Future Scope

- **Plain Text Security:** Passwords are stored in plain text within the binary file (no encryption/hashing applied).
- **Hardcoded Limits:** Fixed user limit set in memory (`MAX_USERS = 100`).
- **Concurrency:** Single-user system intended for local execution (no multi-threading or race-condition handling).
- **Platform-Dependent Code:** Uses `system("cls")` for screen clearing, which requires a minor manual adjustment for Unix-based systems (see below).

---

## 🔧 How to Run

### Prerequisites
You need a C compiler (like GCC) installed on your system.

### OS Specific Setup
The code clears the console screen for a cleaner UI. By default, it uses the Windows command. 
* **For Windows:** No changes needed.
* **For Linux / macOS:** Open `bank.c`, scroll to the bottom, and update the `clear_screen()` function:
  ```c
  void clear_screen() {
      // system("cls"); // Comment this out
      system("clear"); // Uncomment this for Unix os
  }
