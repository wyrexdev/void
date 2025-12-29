#include "Engine/Parser/Html/Tokenizer.hpp"

namespace HTML
{
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

            std::string tag = html.substr(startTag + 1, endTag - startTag - 1);

            std::unordered_map<std::string, std::string> attributes;
            size_t nameEnd = tag.find_first_of(" \t\n\r");
            std::string tagName = (nameEnd == std::string::npos) ? tag : tag.substr(0, nameEnd);

            if (!tagName.starts_with("/") && nameEnd != std::string::npos)
            {
                std::string attrStr = tag.substr(nameEnd + 1);
                size_t attrPos = 0;

                while (attrPos < attrStr.length())
                {
                    while (attrPos < attrStr.length() &&
                           (attrStr[attrPos] == ' ' || attrStr[attrPos] == '\t' ||
                            attrStr[attrPos] == '\n' || attrStr[attrPos] == '\r'))
                    {
                        attrPos++;
                    }

                    if (attrPos >= attrStr.length())
                        break;

                    size_t eqPos = attrStr.find('=', attrPos);
                    if (eqPos == std::string::npos)
                        break;

                    std::string attrName = attrStr.substr(attrPos, eqPos - attrPos);

                    size_t nameEndPos = attrName.find_last_not_of(" \t\n\r");
                    if (nameEndPos != std::string::npos)
                        attrName = attrName.substr(0, nameEndPos + 1);

                    attrPos = eqPos + 1;

                    while (attrPos < attrStr.length() &&
                           (attrStr[attrPos] == ' ' || attrStr[attrPos] == '\t' ||
                            attrStr[attrPos] == '\n' || attrStr[attrPos] == '\r'))
                    {
                        attrPos++;
                    }

                    if (attrPos >= attrStr.length())
                        break;

                    char quoteChar = attrStr[attrPos];
                    bool hasQuotes = (quoteChar == '\'' || quoteChar == '"');

                    if (hasQuotes)
                    {
                        attrPos++;
                        size_t quoteEnd = attrStr.find(quoteChar, attrPos);
                        if (quoteEnd == std::string::npos)
                            break;

                        std::string attrValue = attrStr.substr(attrPos, quoteEnd - attrPos);
                        attributes[attrName] = attrValue;

                        attrPos = quoteEnd + 1;
                    }
                    else
                    {
                        size_t valueEnd = attrStr.find_first_of(" \t\n\r", attrPos);
                        if (valueEnd == std::string::npos)
                            valueEnd = attrStr.length();

                        std::string attrValue = attrStr.substr(attrPos, valueEnd - attrPos);
                        attributes[attrName] = attrValue;

                        attrPos = valueEnd;
                    }
                }
            }

            if (tagName == "script" || (tagName.starts_with("script") && tagName.length() > 6 && !std::isalnum(tagName[6])))
            {
                size_t scriptEnd = html.find("</script>", endTag + 1);
                if (scriptEnd == std::string::npos)
                    break;

                std::string scriptContent = html.substr(endTag + 1, scriptEnd - endTag - 1);

                Token token;
                token.type = TokenType::Tag;
                token.name = tagName;
                token.attributes = attributes;
                token.content = scriptContent;

                tokens.push_back(token);

                pos = scriptEnd + 9;
            }
            else if (!tag.starts_with("/"))
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
                token.name = tagName;
                token.attributes = attributes;
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
} // namespace HTML
