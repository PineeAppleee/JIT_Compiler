#include "parser.h"
#include <stdexcept>
#include <map> 

static const std::map<std::string, OpCode> opcodeMap = {
    {"PUSH", OpCode::PUSH}, {"ADD", OpCode::ADD}, {"SUB", OpCode::SUB},
    {"MUL", OpCode::MUL}, {"DIV", OpCode::DIV}, {"MOD", OpCode::MOD},
    {"STORE", OpCode::STORE}, {"LOAD", OpCode::LOAD}, {"IF", OpCode::IF},
    {"GOTO", OpCode::GOTO}, {"FUNC", OpCode::FUNC}, {"CALL", OpCode::CALL},
    {"RET", OpCode::RET}, {"PRINT", OpCode::PRINT}, {"ARRAY", OpCode::ARRAY},
    {"ARRANGE", OpCode::ARRANGE}
};

static OpCode strToOpCode(const std::string& s) {
    auto it = opcodeMap.find(s);
    if (it != opcodeMap.end()) {
        return it->second;
    }
    throw std::runtime_error("Unknown opcode: " + s);
}

static const std::map<OpCode, int> opcodeArgCount = {
    {OpCode::PUSH, 1}, {OpCode::STORE, 1}, {OpCode::LOAD, 1}, {OpCode::GOTO, 1},
    {OpCode::FUNC, 1}, {OpCode::CALL, 1},
    {OpCode::ARRAY, -1}, 
    {OpCode::ADD, 0}, {OpCode::SUB, 0}, {OpCode::MUL, 0}, {OpCode::DIV, 0}, {OpCode::MOD, 0},
    {OpCode::IF, 0}, {OpCode::RET, 0}, {OpCode::PRINT, 0}, {OpCode::ARRANGE, 0}
};

std::vector<Instruction> Parser::parse(const std::vector<std::string>& tokens) {
    std::vector<Instruction> program;
    for (size_t i = 0; i < tokens.size(); ++i) {
        const std::string& token = tokens[i];
        
        if (!token.empty() && token.back() == ':') {
            continue; 
        }

        try {
            OpCode op = strToOpCode(token);
            auto it_arg_count = opcodeArgCount.find(op);
            if (it_arg_count == opcodeArgCount.end()) {
                 throw std::runtime_error("Internal error: Opcode " + token + " not defined in argument map.");
            }
            int expected_args = it_arg_count->second;

            if (expected_args == 1) {
                if (i + 1 >= tokens.size()) throw std::runtime_error(token + " missing argument");
                program.push_back({op, {tokens[i+1]}});
                i += 1; 
            } else if (expected_args == 0) {
                program.push_back({op, {}});
            } else if (op == OpCode::ARRAY) {
                if (i + 1 >= tokens.size() || tokens[i+1] != "[") throw std::runtime_error("ARRAY missing '['");
                std::vector<std::string> arr_args;
                size_t j = i + 2;
                while (j < tokens.size() && tokens[j] != "]") {  
                    if (tokens[j] != ",") {
                        arr_args.push_back(tokens[j]);
                    }
                    j++;
                }
                if (j >= tokens.size() || tokens[j] != "]") throw std::runtime_error("ARRAY missing ']'");
                program.push_back({OpCode::ARRAY, arr_args});
                i = j; 
            }
        } catch (const std::runtime_error&) {
            continue;
        }
    }
    return program;
}
