
#include "parser.h"
#include "vm.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

std::vector<std::string> tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::string token;
    for (char ch : input) {
        if (isspace(ch)) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else if (ch == '[' || ch == ']' || ch == ',') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            tokens.emplace_back(1, ch);
        } else if (ch == ';') {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        } else {
            token += ch;
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }
    return tokens;
}

int main() {
    std::cout << "Enter your script (end with Ctrl+D):\n";
    std::string line, script;
    while (std::getline(std::cin, line)) {
        script += line + " ";
    }
    auto tokens = tokenize(script);
    Parser parser;
    auto program = parser.parse(tokens);
    VM vm;
    vm.load(program);
    vm.run();
    return 0;
}