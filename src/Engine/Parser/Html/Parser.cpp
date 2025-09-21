#include "Engine/Parser/Html/Parser.hpp"

Parser::Parser() {
    
}

void Parser::parse(std::string &html) {
    Tokenizer tokenizer;
    auto tokens = tokenizer.tokenize(html);

    for(auto &t : tokens) {
        switch(t.type) {
            // case TokenType::StartTag: std::cout << "StartTag: " << t.name << "\n"; break;
            // case TokenType::EndTag: std::cout << "EndTag: " << t.name << "\n"; break;
            // case TokenType::Content: std::cout << "Content: " << t.content << "\n"; break;
            default: break;
        }
    }
}