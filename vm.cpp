#include "vm.h"
#include <iostream>

void VM::load(const std::vector<Instruction>& prog) {
    program = prog;
    stack.clear();
    array_stack.clear();
    variables.clear();
    labels.clear();
    call_stack.clear();
    for (size_t i = 0; i < program.size(); ++i) {
        if (program[i].op == OpCode::FUNC || program[i].op == OpCode::GOTO) {
            if (!program[i].args.empty()) {
                labels[program[i].args[0]] = i;
            }
        }
    }
    ip = 0;
}

void VM::run() {
    while (ip < program.size()) {
        const Instruction& instr = program[ip];
        switch (instr.op) {
            case OpCode::PUSH: {
                stack.push_back(std::stoi(instr.args[0]));
                ++ip;
                break;
            }
            case OpCode::ADD: {
                int b = stack.back(); stack.pop_back();
                int a = stack.back(); stack.pop_back();
                stack.push_back(a + b);
                ++ip;
                break;
            }
            case OpCode::SUB: {
                int b = stack.back(); stack.pop_back();
                int a = stack.back(); stack.pop_back();
                stack.push_back(a - b);
                ++ip;
                break;
            }
            case OpCode::MUL: {
                int b = stack.back(); stack.pop_back();
                int a = stack.back(); stack.pop_back();
                stack.push_back(a * b);
                ++ip;
                break;
            }
            case OpCode::STORE:
                variables[instr.args[0]] = stack.back();
                stack.pop_back();
                ++ip;
                break;
            case OpCode::LOAD:
                stack.push_back(variables[instr.args[0]]);
                ++ip;
                break;
            case OpCode::IF: {
                int cond = stack.back(); stack.pop_back();
                if (cond) {
                    ++ip;
                } else {
                    ip += 2;
                }
                break;
            }
            case OpCode::GOTO:
                ip = labels[instr.args[0]];
                break;
            case OpCode::FUNC:
                ++ip;
                break;
            case OpCode::CALL:
                call_stack.push_back(ip + 1);
                ip = labels[instr.args[0]];
                break;
            case OpCode::RET:
                ip = call_stack.back();
                call_stack.pop_back();
                break;
            case OpCode::PRINT: {
                if (!array_stack.empty()) {
                    const auto& arr = array_stack.back();
                    std::cout << "[";
                    for (size_t i = 0; i < arr.size(); ++i) {
                        std::cout << arr[i];
                        if (i + 1 < arr.size()) std::cout << ", ";
                    }
                    std::cout << "]" << std::endl;
                } else if (!stack.empty()) {
                    std::cout << stack.back() << std::endl;
                } else {
                    std::cerr << "Stack is empty, nothing to print." << std::endl;
                }
                ++ip;
                break;
            }
            case OpCode::ARRAY: {
                std::vector<int> arr;
                for (const auto& s : instr.args) {
                    arr.push_back(std::stoi(s));
                }
                array_stack.push_back(arr);
                ++ip;
                break;
            }
            case OpCode::ARRANGE: {
                if (!array_stack.empty()) {
                    std::sort(array_stack.back().begin(), array_stack.back().end());
                } else {
                    std::cerr << "Array stack is empty, nothing to arrange." << std::endl;
                }
                ++ip;
                break;
            }
            default:
                std::cerr << "Unknown opcode!\n";
                return;
        }
    }
}