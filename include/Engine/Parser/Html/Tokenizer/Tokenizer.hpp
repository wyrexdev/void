#pragma once

#include <vector>
#include <string>

#include "Engine/Parser/Html/Tokenizer/Token.hpp"

class Tokenizer {
public:
    std::vector<Token> tokenize(const std::string& html);
};