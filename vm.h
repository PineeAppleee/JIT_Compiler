#pragma once
#include <string>
#include <vector>
#include <unordered_map>

enum class OpCode {
    PUSH, ADD, SUB, STORE, LOAD, IF, GOTO, FUNC, CALL, RET, MUL, DIV, MOD, PRINT, ARRAY, ARRANGE
};

struct Instruction {
    OpCode op;
    std::vector<std::string> args;
};

class VM {
public:
    void load(const std::vector<Instruction>& program, const std::vector<std::string>& tokens);
    
    std::string run_and_serialize();

private:
    void run();

    std::vector<Instruction> program;
    std::vector<int> stack;
    std::vector<std::vector<int>> array_stack;
    std::unordered_map<std::string, int> variables;
    std::unordered_map<std::string, size_t> labels;
    std::vector<size_t> call_stack;
    size_t ip = 0;
    std::string output_buffer; 
};
