#include "parser.h"
#include "vm.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>


std::vector<std::string> tokenize(const std::string& input) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string line;

    while (std::getline(ss, line)) {
        std::string token_line = line.substr(0, line.find(';')); 
        std::stringstream line_ss(token_line);
        std::string token;
        while (line_ss >> token) {
            tokens.push_back(token);
        }
    }
    return tokens;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "{\"error\": \"Usage: " << argv[0] << " \\\"<script>\\\"\"}" << std::endl;
        return 1;
    }

    std::string script = argv[1];
    
    try {
        auto tokens = tokenize(script);
        Parser parser;
        auto program = parser.parse(tokens);
        VM vm;
        vm.load(program, tokens);
        std::string json_output = vm.run_and_serialize();
        std::cout << json_output << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "{\"error\": \"" << e.what() << "\"}" << std::endl;
        return 1;
    }

    return 0;
}
