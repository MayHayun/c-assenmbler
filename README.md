# Assembly to Machine Language Converter

## Project Overview

This project is a simple command-line tool written in C that takes an input file containing assembly code, checks its syntax for correctness, and converts the valid assembly code into machine language. The output is a file named `run.exe`, which contains the machine language code in hexadecimal format.

## Features

- Syntax checking for valid assembly code.
- Assembly to machine language conversion.
- Output in hexadecimal format.

## Usage

1. **Compile the Project:**
   - Ensure you have a C compiler installed (e.g., GCC).
   - Compile the project using the provided Makefile:

     ```bash
     make
     ```

   This will generate the executable file named `run.exe`.

2. **Run the Assembler:**
   - Execute the compiled program, providing the input assembly file as an argument:

     ```bash
     ./run.exe input.asm
     ```

   Replace `input.asm` with the path to your assembly code file.

3. **Check Output:**
   - If the assembly code is syntactically correct, the program will generate a file named `output.hex` containing the machine language code in hexadecimal format.



