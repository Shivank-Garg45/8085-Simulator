# 8085 Simulator (Terminal Based)

This project is a **terminal-based Intel 8085 microprocessor simulator written in C**.
It simulates the execution of 8085 assembly instructions and displays the internal CPU state such as registers, flags, and memory during program execution.

The simulator reads an assembly program from a file and executes it step-by-step, helping students understand how an **8085 microprocessor works internally**.

---

## Features

* Simulation of **8085 CPU registers**
* Simulation of **flag register**
* **64KB memory simulation**
* Support for **assembly instructions**
* **Label support for jump instructions**
* **Step-by-step execution mode**
* **Memory viewing after program execution**

---

## CPU Registers

The simulator supports the following registers:

* A (Accumulator)
* B, C
* D, E
* H, L
* Program Counter (PC)
* Stack Pointer (SP)

### Flags

* Zero (Z)
* Sign (S)
* Parity (P)
* Carry (CY)
* Auxiliary Carry (AC)

---

## Project Structure

```
8085-Simulator
│
├── main.c
├── cpu.c
├── memory.c
├── instructions.c
├── parser.c
├── simulator.h
├── program.asm
└── README.md
```

### File Description

**main.c**
Entry point of the program. Initializes CPU and runs the simulator.

**cpu.c**
Handles CPU initialization, flag updates, and printing CPU state.

**memory.c**
Implements the 64KB memory used by the simulator.

**instructions.c**
Contains implementation of 8085 instructions.

**parser.c**
Parses assembly instructions and executes them.

**simulator.h**
Contains structure definitions and function declarations.

---

## How to Compile

Use **GCC** to compile the project.

```
gcc main.c cpu.c memory.c parser.c instructions.c -o sim
```

---

## How to Run

### Linux / Mac

```
./sim
```

### Windows

```
sim.exe
```

---

## Example Assembly Program

Create a file named **program.asm**

```
MVI A, 05
MVI B, 03
ADD B
STA 2000
HLT
```

This program adds two numbers and stores the result in memory.

---

## Output Example

```
Executing Line 1: MVI A, 05

================ CPU STATE ================
A:05  B:00  C:00  D:00  E:00  H:00  L:00
PC:0000  SP:FFFF
FLAGS -> Z:0 S:0 P:1 CY:0 AC:0
===========================================
```

---

## Purpose

This project was created for learning and understanding:

* Microprocessors
* Assembly language
* CPU instruction execution
* Memory operations

---

## Author

**Shivank Garg**
