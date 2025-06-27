#include "vm.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <algorithm> 
#include <map>

void VM::load(const std::vector<Instruction>& prog, const std::vector<std::string>& tokens) {
    program = prog;
    stack.clear();
    array_stack.clear();
    variables.clear();
    labels.clear();
    call_stack.clear();
    output_buffer.clear();
    ip = 0;
    int instruction_index = 0;
    const std::map<std::string, OpCode> tempOpcodeMap = {
        {"PUSH", OpCode::PUSH}, {"ADD", OpCode::ADD}, {"SUB", OpCode::SUB},
        {"MUL", OpCode::MUL}, {"DIV", OpCode::DIV}, {"MOD", OpCode::MOD},
        {"STORE", OpCode::STORE}, {"LOAD", OpCode::LOAD}, {"IF", OpCode::IF},
        {"GOTO", OpCode::GOTO}, {"FUNC", OpCode::FUNC}, {"CALL", OpCode::CALL},
        {"RET", OpCode::RET}, {"PRINT", OpCode::PRINT}, {"ARRAY", OpCode::ARRAY},
        {"ARRANGE", OpCode::ARRANGE}
    };

    for (const auto& token : tokens) {
        if (!token.empty() && token.back() == ':') {
            labels[token.substr(0, token.length() - 1)] = instruction_index;
        } else if (tempOpcodeMap.count(token)) {
            instruction_index++;
        }
    }
}

void VM::run() {
    while (ip < program.size()) {
        const Instruction& instr = program[ip];
        switch (instr.op) {
            case OpCode::PUSH:
                stack.push_back(std::stoi(instr.args[0]));
                ++ip;
                break;
            case OpCode::ADD: {
                if (stack.size() < 2) throw std::runtime_error("Stack underflow for ADD");
                int b = stack.back(); stack.pop_back();
                int a = stack.back(); stack.pop_back();
                stack.push_back(a + b);
                ++ip;
                break;
            }
            case OpCode::SUB: {
                if (stack.size() < 2) throw std::runtime_error("Stack underflow for SUB");
                int b = stack.back(); stack.pop_back();
                int a = stack.back(); stack.pop_back();
                stack.push_back(a - b);
                ++ip;
                break;
            }
            case OpCode::MUL: {
                 if (stack.size() < 2) throw std::runtime_error("Stack underflow for MUL");
                int b = stack.back(); stack.pop_back();
                int a = stack.back(); stack.pop_back();
                stack.push_back(a * b);
                ++ip;
                break;
            }
            case OpCode::STORE:
                if (stack.empty()) throw std::runtime_error("Stack underflow for STORE");
                variables[instr.args[0]] = stack.back();
                stack.pop_back();
                ++ip;
                break;
            case OpCode::LOAD:
                if (variables.find(instr.args[0]) == variables.end()) throw std::runtime_error("Variable not found for LOAD: " + instr.args[0]);
                stack.push_back(variables.at(instr.args[0]));
                ++ip;
                break;
            case OpCode::IF: {
                if (stack.empty()) throw std::runtime_error("Stack underflow for IF");
                int cond = stack.back(); stack.pop_back();
                if (cond > 0) {
                    ++ip;
                } else {
                    ip += 2; 
                }
                break;
            }
            case OpCode::GOTO:
                 if (labels.find(instr.args[0]) == labels.end()) throw std::runtime_error("Label not found for GOTO: " + instr.args[0]);
                ip = labels.at(instr.args[0]);
                break;
            case OpCode::FUNC:
                ++ip;
                break;
            case OpCode::CALL:
                 if (labels.find(instr.args[0]) == labels.end()) throw std::runtime_error("Function not found for CALL: " + instr.args[0]);
                call_stack.push_back(ip + 1);
                ip = labels.at(instr.args[0]) + 1;
                break;
            case OpCode::RET:
                if (call_stack.empty()) throw std::runtime_error("Cannot RET from outside a function call.");
                ip = call_stack.back();
                call_stack.pop_back();
                break;
            case OpCode::PRINT: {
                std::stringstream ss;
                if (!array_stack.empty()) {
                    const auto& arr = array_stack.back();
                    ss << "[";
                    for (size_t i = 0; i < arr.size(); ++i) {
                        ss << arr[i] << (i + 1 < arr.size() ? ", " : "");
                    }
                    ss << "]";
                } else if (!stack.empty()) {
                    ss << stack.back();
                } else {
                    ss << "Stack is empty, nothing to print.";
                }
                output_buffer += ss.str() + "\\n"; 
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
                    output_buffer += "Array stack is empty, nothing to arrange.\\n";
                }
                ++ip;
                break;
            }
            default:
                throw std::runtime_error("Unknown opcode during execution!");
        }
    }
}

std::string VM::run_and_serialize() {
    run(); 
    std::stringstream json;
    json << "{";
    json << "\"stack\": [";
    for (size_t i = 0; i < stack.size(); ++i) {
        json << stack[i] << (i + 1 < stack.size() ? ", " : "");
    }
    json << "], ";
    json << "\"variables\": {";
    for (auto it = variables.begin(); it != variables.end(); ++it) {
        json << "\"" << it->first << "\": " << it->second;
        if (std::next(it) != variables.end()) {
            json << ", ";
        }
    }
    json << "}, ";
    json << "\"array_stack\": [";
    for (size_t i = 0; i < array_stack.size(); ++i) {
        json << "[";
        for (size_t j = 0; j < array_stack[i].size(); ++j) {
            json << array_stack[i][j] << (j + 1 < array_stack[i].size() ? ", " : "");
        }
        json << "]" << (i + 1 < array_stack.size() ? ", " : "");
    }
    json << "], ";
    json << "\"call_stack\": [";
    for (size_t i = 0; i < call_stack.size(); ++i) {
        json << call_stack[i] << (i + 1 < call_stack.size() ? ", " : "");
    }
    json << "], ";
    json << "\"output\": \"" << output_buffer << "\"";
    json << "}";
    return json.str();
}
