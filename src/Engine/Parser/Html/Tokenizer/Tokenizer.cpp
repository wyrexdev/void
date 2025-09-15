#include "Engine/Parser/Html/Tokenizer/Tokenizer.hpp"

std::vector<Token> Tokenizer::tokenize(const std::string &html) {
    std::vector<Token> tokens;
    std::string buffer;

    bool inTag = false;
    bool isEndTag = false;

    for(size_t i = 0; i < html.size(); ++i) {
        char c = html[i];

        if(c == '<') {
            if(!buffer.empty()) {
                tokens.push_back({ TokenType::Content, "", {}, buffer });
                buffer.clear();
            }

            inTag = true;
            isEndTag = (html[i+1] == '/');
            if(isEndTag) i++;
        } else if (c == '>') {
            if(inTag) {
                std::string tagName = buffer;
                buffer.clear();

                Token t;
                t.name = tagName;
                t.type = isEndTag ? TokenType::EndTag : TokenType::StartTag;
                tokens.push_back(t);

                inTag = false;
            }
        } else {
            buffer += c;
        }
    }

    if(!buffer.empty()) {
        tokens.push_back({ TokenType::Content, "", {}, buffer });
    }

    return tokens;
}