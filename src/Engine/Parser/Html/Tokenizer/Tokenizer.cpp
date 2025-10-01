#include "Engine/Parser/Html/Tokenizer/Tokenizer.hpp"

std::vector<Token> Tokenizer::tokenize(const std::string &html) {
    std::vector<Token> tokens;
    enum State { Outside, InsideTag, InsideScriptStyle } state = Outside;

    std::string currentTag;
    std::string currentText;
    size_t i = 0;

    while (i < html.size()) {
        char c = html[i];

        if (state == Outside) {
            if (c == '<') {
                if (!currentText.empty()) {
                    Token t;
                    t.type = TokenType::Content;
                    t.content = currentText;
                    tokens.push_back(t);
                    currentText.clear();
                }
                state = InsideTag;
                currentTag = "<";
            } else {
                currentText += c;
            }
            ++i;
        } else if (state == InsideTag) {
            currentTag += c;
            if (c == '>') {
                Token t;
                if (currentTag[1] == '/') {
                    t.type = TokenType::EndTag;
                } else {
                    t.type = TokenType::StartTag;
                }
                t.name = currentTag;

                if (t.type == TokenType::StartTag &&
                    (currentTag.substr(0,7) == "<script" || currentTag.substr(0,6) == "<style")) {
                    state = InsideScriptStyle;
                } else {
                    state = Outside;
                }

                tokens.push_back(t);
                currentTag.clear();
                ++i;
            } else {
                ++i;
            }
        } else if (state == InsideScriptStyle) {
            size_t endTagPos = std::string::npos;
            if (html.substr(i, 9) == "</script>") endTagPos = i;
            else if (html.substr(i, 8) == "</style>") endTagPos = i;

            if (endTagPos != std::string::npos) {
                Token t;
                t.type = TokenType::Content;
                t.content = html.substr(0, endTagPos - i + i);
                tokens.push_back(t);

                i = endTagPos;
                state = InsideTag;
            } else {
                ++i;
            }
        }
    }

    if (!currentText.empty()) {
        Token t;
        t.type = TokenType::Content;
        t.content = currentText;
        tokens.push_back(t);
    }

    return tokens;
}