# 8085 Simulator

An Intel 8085 microprocessor simulator written in C with both a **Terminal Interface** and a **Raylib Graphical Interface**.

The simulator executes 8085 assembly programs and displays CPU registers, flags, memory, and program execution step-by-step.

## Features

- 8085 CPU simulation

- 64KB memory simulation

- CPU registers: A, B, C, D, E, H, L

- Program Counter (PC)

- Stack Pointer (SP)

- Flag simulation: Z, S, P, CY, AC

- Assembly instruction parsing

- Label support

- Step-by-step execution

- Memory viewer

- Assembly program editor

- Load, Run, Step, Reset and Stop controls

- Raylib-based graphical interface

- Terminal-based interface

## Project Structure

```
8085-Simulator/
├── main.c
├── cpu.c
├── memory.c
├── instructions.c
├── parser.c
├── simulator.h
├── program.asm
├── Makefile
├── raylib_test.c
├── gui/
│   ├── gui.c
│   └── gui_globals.c
├── .gitignore
└── README.md
```

## Technologies

- C
- Raylib
- GCC
- Make
- Git
- GitHub

# Run:

./sim
Build GUI Version

Make sure Raylib is installed.

gcc gui/gui.c gui/gui_globals.c cpu.c memory.c instructions.c parser.c -o 8085_gui $(pkg-config --cflags --libs raylib) -lm -lX11

# Run:

./8085_gui

```
## Build GUI Version

- Build using the Makefile:
make

```

# Example Assembly Program:

MVI A, 5AH

STA 2050H

MVI A, 00H

LDA 2050H

MOV B, A

HLT

# After execution:

A = 5A

B = 5A

Memory[2050H] = 5A


# The Raylib GUI provides:

- Assembly program display and editing
- CPU register monitoring
- Flag monitoring
- Memory inspection
- Program loading
- Step-by-step execution
- Program execution controls
- Current instruction tracking
- Learning Objectives

# This project helps understand:

- 8085 microprocessor architecture
- Assembly language
- CPU registers and flags
- Memory operations
- Instruction execution
- Program flow and branching
- Low-level system concepts
- GUI programming with Raylib
- Future Improvements
- Syntax highlighting
- Breakpoints
- Improved error handling
- Better memory visualization
- Register editing
- Instruction information panel
- Execution speed control
- More complete 8085 instruction support
- CMake support

# Author
Shivank Garg
