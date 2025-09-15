#include "Engine/Parser/Html/Tokenizer/Document.hpp"
#include "Engine/Parser/Html/Tokenizer/Tokenizer.hpp"

#include <iostream>

class Parser {
public:
    Parser();

    void parse(std::string &html);
};