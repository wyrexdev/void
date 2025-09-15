#pragma once
#include <string>
#include <unordered_map>

enum class TokenType
{
    StartTag,
    EndTag,
    Content,
    Comment,
    Doctype
};

struct Token
{
    TokenType type;
    std::string name;
    std::unordered_map<std::string, std::string> attributes;
    std::string content;
};
