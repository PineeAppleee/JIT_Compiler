#include "parser.h"
#include <stdexcept>

static OpCode strToOpCode(const std::string& s) {
    if (s == "PUSH") return OpCode::PUSH;
    if (s == "ADD") return OpCode::ADD;
    if (s == "SUB") return OpCode::SUB;
    if (s == "MUL") return OpCode::MUL;
    if (s == "STORE") return OpCode::STORE;
    if (s == "LOAD") return OpCode::LOAD;
    if (s == "IF") return OpCode::IF;
    if (s == "GOTO") return OpCode::GOTO;
    if (s == "FUNC") return OpCode::FUNC;
    if (s == "CALL") return OpCode::CALL;
    if (s == "RET") return OpCode::RET;
    if (s == "PRINT") return OpCode::PRINT;
    if (s == "ARRAY") return OpCode::ARRAY;
    if (s == "ARRANGE") return OpCode::ARRANGE;
    throw std::runtime_error("Unknown opcode: " + s);
}

std::vector<Instruction> Parser::parse(const std::vector<std::string>& tokens) {
    std::vector<Instruction> program;
    for (size_t i = 0; i < tokens.size(); ++i) {
        const std::string& token = tokens[i];
        if (!token.empty() && token.back() == ':') {
            continue;
        }
        if (token == "PUSH") {
            if (i + 1 >= tokens.size()) throw std::runtime_error("PUSH missing argument");
            program.push_back({OpCode::PUSH, {tokens[i+1]}});
            i += 1;
        } else if (token == "STORE") {
            if (i + 1 >= tokens.size()) throw std::runtime_error("STORE missing argument");
            program.push_back({OpCode::STORE, {tokens[i+1]}});
            i += 1;
        } else if (token == "LOAD") {
            if (i + 1 >= tokens.size()) throw std::runtime_error("LOAD missing argument");
            program.push_back({OpCode::LOAD, {tokens[i+1]}});
            i += 1;
        } else if (token == "GOTO") {
            if (i + 1 >= tokens.size()) throw std::runtime_error("GOTO missing argument");
            program.push_back({OpCode::GOTO, {tokens[i+1]}});
            i += 1;
        } else if (token == "FUNC") {
            if (i + 1 >= tokens.size()) throw std::runtime_error("FUNC missing argument");
            program.push_back({OpCode::FUNC, {tokens[i+1]}});
            i += 1;
        } else if (token == "CALL") {
            if (i + 1 >= tokens.size()) throw std::runtime_error("CALL missing argument");
            program.push_back({OpCode::CALL, {tokens[i+1]}});
            i += 1;
        } else if (token == "ADD") {
            program.push_back({OpCode::ADD, {}});
        } else if (token == "SUB") {
            program.push_back({OpCode::SUB, {}});
        } else if (token == "MUL") {
            program.push_back({OpCode::MUL, {}});
        } else if (token == "IF") {
            program.push_back({OpCode::IF, {}});
        } else if (token == "RET") {
            program.push_back({OpCode::RET, {}});
        } else if (token == "PRINT") {
            program.push_back({OpCode::PRINT, {}});
        } else if (token == "ARRAY") {
            if (i + 1 >= tokens.size() || tokens[i+1] != "[") throw std::runtime_error("ARRAY missing '['");
            std::vector<std::string> arr;
            i += 2; 
            while (i < tokens.size() && tokens[i] != "]") {
                if (tokens[i] != ",") {
                    arr.push_back(tokens[i]);
                }
                ++i;
            }
            if (i >= tokens.size() || tokens[i] != "]") throw std::runtime_error("ARRAY missing ']'");
            program.push_back({OpCode::ARRAY, arr});
        } else if (token == "ARRANGE") {
            program.push_back({OpCode::ARRANGE, {}});
        } else {
            throw std::runtime_error("Unknown token: " + token);
        }
    }
    return program;
}