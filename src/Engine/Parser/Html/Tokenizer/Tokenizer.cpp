#include "Engine/Parser/Html/Tokenizer/Tokenizer.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

std::vector<Token> Tokenizer::tokenize(const std::string &html)
{
    std::vector<Token> tokens;

    size_t pos = 0;
    while (pos < html.length())
    {
        size_t startTag = html.find('<', pos);
        if (startTag == std::string::npos)
            break;

        size_t endTag = html.find('>', startTag);
        if (endTag == std::string::npos)
            break;

        std::string tag = html.substr(startTag, endTag - startTag);

        if (tag.starts_with("<script") && !tag.starts_with("</script"))
        {
            size_t scriptEnd = html.find("</script>", endTag + 1);
            if (scriptEnd == std::string::npos)
                break;

            std::string scriptContent = html.substr(endTag + 1, scriptEnd - endTag - 1);
            
            Token token;
            token.type = TokenType::Tag;
            token.name = (split(((split(tag, ' '))[0]), '<'))[1];
            token.content = scriptContent;

            tokens.push_back(token);

            pos = scriptEnd + 9;
        }
        else if (!tag.starts_with("</"))
        {
            size_t contentStart = endTag + 1;
            size_t nextTag = html.find('<', contentStart);

            std::string content;
            if (nextTag != std::string::npos)
                content = html.substr(contentStart, nextTag - contentStart);
            else
                content = html.substr(contentStart);

            Token token;
            token.type = TokenType::Tag;
            token.name = (split(((split(tag, ' '))[0]), '<'))[1];
            token.content = content;

            tokens.push_back(token);

            pos = nextTag;
        }
        else
        {
            pos = endTag + 1;
        }
    }

    return tokens;
}

std::vector<std::string> Tokenizer::split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(s);
    std::string token;

    while (std::getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}