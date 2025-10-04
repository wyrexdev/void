#include "Engine/Request/Fetcher.hpp"

size_t Fetcher::writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string Fetcher::convertToUTF8(const std::string& input, const std::string& fromCharset) {
    iconv_t cd = iconv_open("UTF-8", fromCharset.c_str());
    if (cd == (iconv_t)-1) return input;

    size_t inBytes = input.size();
    size_t outBytes = inBytes * 4;
    std::vector<char> outBuf(outBytes);

    char* inBuf = const_cast<char*>(input.c_str());
    char* outPtr = outBuf.data();

    if (iconv(cd, &inBuf, &inBytes, &outPtr, &outBytes) == (size_t)-1) {
        iconv_close(cd);
        return input;
    }

    std::string result(outBuf.data(), outPtr - outBuf.data());
    iconv_close(cd);
    return result;
}

std::string Fetcher::get(const std::string& addr) {
    CURL* curl = curl_easy_init();
    std::string readBuffer;

    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, addr.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Fetcher::writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, curl_slist_append(NULL, "Accept-Charset: utf-8, ISO-8859-9, windows-1254"));

        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;

        curl_easy_cleanup(curl);
    }

    bool isUTF8 = true;
    for (unsigned char c : readBuffer) {
        if (c >= 128 && c < 192) { isUTF8 = false; break; }
    }

    if (!isUTF8) {
        std::string converted = convertToUTF8(readBuffer, "ISO-8859-9");
        if (converted == readBuffer)
            converted = convertToUTF8(readBuffer, "WINDOWS-1254");
        return converted;
    }

    return readBuffer;
}
