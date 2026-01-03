#include "Engine/Request/NetworkLoader.hpp"

size_t NetworkLoader::writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

size_t NetworkLoader::headerCallback(char *buffer, size_t size, size_t nitems, void *userdata)
{
    size_t total = size * nitems;
    std::string header_line(buffer, total);

    auto *headers = static_cast<std::unordered_map<std::string, std::string> *>(userdata);

    auto pos = header_line.find(':');
    if (pos != std::string::npos)
    {
        std::string key = header_line.substr(0, pos);
        std::string value = header_line.substr(pos + 1);

        key.erase(key.find_last_not_of(" \n\r\t") + 1);
        value.erase(0, value.find_first_not_of(" \n\r\t"));
        value.erase(value.find_last_not_of(" \n\r\t") + 1);

        (*headers)[key] = value;
    }

    return total;
}

std::string NetworkLoader::ensureUTF8(const std::string &content, const char *content_type)
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

bool NetworkLoader::isValidUTF8(const std::string &str)
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

std::string NetworkLoader::convertToUTF8(const std::string &input, const std::string &fromCharset)
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

void NetworkLoader::parseContentType(const char *ct, std::string &mime, std::string &charset)
{
    if (!ct)
        return;

    std::string contentType(ct);
    auto semi = contentType.find(';');

    if (semi != std::string::npos)
    {
        mime = contentType.substr(0, semi);

        auto charsetPos = contentType.find("charset=");
        if (charsetPos != std::string::npos)
        {
            charset = contentType.substr(charsetPos + 8);
        }
    }
    else
    {
        mime = contentType;
    }

    mime.erase(mime.find_last_not_of(" \n\r\t") + 1);
    charset.erase(charset.find_last_not_of(" \n\r\t") + 1);
}

Core::Resource NetworkLoader::get(const std::string &addr)
{
    Core::Resource resource{};
    resource.body = "";

    std::string decoded = urlDecode(addr);

    decoded.erase(0, decoded.find_first_not_of(" \n\r\t"));
    decoded.erase(decoded.find_last_not_of(" \n\r\t") + 1);

    if (decoded.find("://") == std::string::npos)
        decoded = "https://" + decoded;

    if (decoded.rfind("https://", 0) == 0)
        resource.origin = Core::ResourceOrigin::HTTPS;
    else if (decoded.rfind("http://", 0) == 0)
        resource.origin = Core::ResourceOrigin::HTTP;

    resource.url = decoded;

    CURL *curl = curl_easy_init();
    if (!curl)
    {
        std::cerr << "Failed to initialize CURL" << std::endl;
        return resource;
    }

    std::string readBuffer;

    curl_easy_setopt(curl, CURLOPT_URL, decoded.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NetworkLoader::writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, NetworkLoader::headerCallback);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &resource.headers);

    curl_easy_setopt(curl, CURLOPT_USERAGENT, "Void/Pre-0.1");
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10L);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
        std::cerr << "curl error: " << curl_easy_strerror(res)
                  << " - URL: [" << decoded << "]" << std::endl;
        curl_easy_cleanup(curl);
        return resource;
    }

    long status = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);
    resource.statusCode = static_cast<int>(status);

    char *contentType = nullptr;
    curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &contentType);
    parseContentType(contentType, resource.mimeType, resource.charset);

    curl_easy_cleanup(curl);

    if (resource.statusCode >= 200 && resource.statusCode < 300)
        resource.body = ensureUTF8(readBuffer, contentType);

    return resource;
}

std::string NetworkLoader::urlDecode(const std::string &input)
{
    if (input.empty())
        return input;

    CURL *curl = curl_easy_init();
    if (!curl)
        return input;

    int outlen = 0;
    char *decoded = curl_easy_unescape(
        curl,
        input.c_str(),
        static_cast<int>(input.size()),
        &outlen
    );

    std::string result;
    if (decoded && outlen > 0)
        result.assign(decoded, outlen);
    else
        result = input;

    if (decoded)
        curl_free(decoded);

    curl_easy_cleanup(curl);
    return result;
}

bool NetworkLoader::isAbsoluteURL(const std::string &url)
{
    return url.rfind("http://", 0) == 0 ||
           url.rfind("https://", 0) == 0 ||
           url.rfind("data:", 0) == 0 ||
           url.rfind("blob:", 0) == 0;
}

std::string NetworkLoader::getOrigin(const std::string &url)
{
    size_t schemeEnd = url.find("://");
    if (schemeEnd == std::string::npos)
        return "";

    schemeEnd += 3;
    size_t slash = url.find('/', schemeEnd);
    if (slash == std::string::npos)
        return url;

    return url.substr(0, slash);
}

std::string NetworkLoader::getBaseDir(const std::string &url)
{
    size_t lastSlash = url.find_last_of('/');
    if (lastSlash == std::string::npos)
        return url + "/";

    return url.substr(0, lastSlash + 1);
}
