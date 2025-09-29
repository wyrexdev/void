#include "Engine/Parser/Html/Tokenizer/Tokenizer.hpp"

std::vector<Token> Tokenizer::tokenize(const std::string &html) {
    std::vector<Token> tokens;
    
    for(int i = 0; i < html.size(); ++i) {
        if(html[i] == '<') {
            const int startTag = i;

            const int endTag = html.find('>', i);
            if(endTag == -1) continue;

            const int nextTag = html.find('<', endTag + 1);
            if(nextTag == -1) continue;

            const std::string tag = html.substr(startTag, endTag);
            const std::string content = html.substr(endTag + 1, nextTag);

            if(!(tag.starts_with("/"))) {
                Token token;
                token.type = TokenType::StartTag;
                token.content = content;
                token.name = tag;

                tokens.push_back(token);
            }
        }
    }

    return tokens;
}