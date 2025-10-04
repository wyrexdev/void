#include "Engine/Request/Fetcher.hpp"

size_t Fetcher::writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

std::string Fetcher::ensureUTF8(const std::string &content, const char *content_type)
{
    if (isValidUTF8(content))
    {
        return content;
    }

    std::string detected_encoding = "ISO-8859-1";

    if (content_type)
    {
        std::string ct(content_type);
        size_t charset_pos = ct.find("charset=");
        if (charset_pos != std::string::npos)
        {
            detected_encoding = ct.substr(charset_pos + 8);
            size_t end = detected_encoding.find(';');
            if (end != std::string::npos)
            {
                detected_encoding = detected_encoding.substr(0, end);
            }
        }
    }

    std::string converted = convertToUTF8(content, detected_encoding);
    if (!converted.empty() && isValidUTF8(converted))
    {
        return converted;
    }

    const char *encodings[] = {"WINDOWS-1252", "ISO-8859-9", "WINDOWS-1254", "ISO-8859-1"};
    for (const char *enc : encodings)
    {
        converted = convertToUTF8(content, enc);
        if (!converted.empty() && isValidUTF8(converted))
        {
            return converted;
        }
    }

    return content;
}

bool Fetcher::isValidUTF8(const std::string &str)
{
    int follow_bytes = 0;
    for (unsigned char c : str)
    {
        if (follow_bytes > 0)
        {
            if ((c & 0xC0) != 0x80)
                return false;
            follow_bytes--;
        }
        else
        {
            if ((c & 0x80) == 0x00)
                continue;
            else if ((c & 0xE0) == 0xC0)
                follow_bytes = 1;
            else if ((c & 0xF0) == 0xE0)
                follow_bytes = 2;
            else if ((c & 0xF8) == 0xF0)
                follow_bytes = 3;
            else
                return false;
        }
    }
    return follow_bytes == 0;
}

std::string Fetcher::convertToUTF8(const std::string &input, const std::string &fromCharset)
{
    iconv_t cd = iconv_open("UTF-8", fromCharset.c_str());
    if (cd == (iconv_t)-1)
        return input;

    size_t inBytes = input.size();
    size_t outBytes = inBytes * 4;
    std::vector<char> outBuf(outBytes);

    char *inBuf = const_cast<char *>(input.c_str());
    char *outPtr = outBuf.data();

    if (iconv(cd, &inBuf, &inBytes, &outPtr, &outBytes) == (size_t)-1)
    {
        iconv_close(cd);
        return input;
    }

    std::string result(outBuf.data(), outPtr - outBuf.data());
    iconv_close(cd);
    return result;
}

std::string Fetcher::get(const std::string &addr)
{
    CURL *curl = curl_easy_init();
    std::string readBuffer;

    if (!curl) {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return "";
    }

    curl_easy_setopt(curl, CURLOPT_URL, addr.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Fetcher::writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Mozilla/5.0 (compatible; Void/1.0)");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);
    
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Accept-Charset: utf-8");
    headers = curl_slist_append(headers, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    CURLcode res = curl_easy_perform(curl);
    
    curl_slist_free_all(headers);
    
    if (res != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        curl_easy_cleanup(curl);
        return "";
    }
    
    char *content_type = nullptr;
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type);
    
    long response_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    
    curl_easy_cleanup(curl);

    if (response_code != 200) {
        std::cerr << "HTTP error: " << response_code << std::endl;
        return "";
    }

    Fetcher f;

    return f.ensureUTF8(readBuffer, content_type);
}