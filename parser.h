#pragma once
#include <vector>
#include <string>
#include "vm.h"

class Parser {
public:
    std::vector<Instruction> parse(const std::vector<std::string>& tokens);
};