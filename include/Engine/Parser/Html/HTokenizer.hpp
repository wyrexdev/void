#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <string>

#include <vector>
#include <string>
#include <unordered_map>

namespace HTML
{
    class Tokenizer
    {
    public:
        struct TokenType
        {
            inline static int Comment = 0;
            inline static int Tag = 1;
        };

        struct Token
        {
            int type;
            std::string name;
            std::unordered_map<std::string, std::string> attributes;
            std::string content;
        };

        std::vector<Token> tokenize(const std::string &html);
        std::vector<std::string> split(const std::string &s, char delimiter);
    };
} // namespace HTML
