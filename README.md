# JIT Compiler

This project is a Just-In-Time (JIT) Compiler implemented in C++. It features a parser, a virtual machine, and a main driver.

## How It Works
The JIT Compiler reads source code, parses it into an intermediate representation, and then executes it using a custom virtual machine. The process involves:
1. **Parsing:** The `parser.cpp` and `parser.h` files handle lexical and syntactic analysis, converting the input code into a structured format.
2. **Compilation:** The parsed code is transformed into bytecode or another intermediate form suitable for execution.
3. **Execution:** The `vm.cpp` and `vm.h` files implement a virtual machine that interprets and runs the compiled code on the fly, providing immediate feedback and results.

This approach allows for efficient execution and rapid development, as code can be tested and run without a separate compilation step.

## Project Structure
- `main.cpp` – Entry point of the compiler
- `parser.cpp` / `parser.h` – Parsing logic
- `vm.cpp` / `vm.h` – Virtual machine implementation

## How to Build
```bash
make
