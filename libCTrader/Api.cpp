//
// Created by Armando Herrera on 3/1/21.
//

#include "Api.h"
#include <iostream>
#include <curl/curl.h>

/* Used by API::Call to put websource into a string type */
static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    ((std::string *) userp)->append((char *) contents, size * nmemb);
    return size * nmemb;
}

std::string Api::call(const std::string &method, bool authed, const std::string &path, const std::string &body) {
    CURL *curl;
    CURLcode res;
    std::string readBuffer;
    curl = curl_easy_init();
    if (curl) {
        struct curl_slist *chunk = nullptr;
        curl_easy_setopt(curl, CURLOPT_URL, (uri + path).c_str());
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl/7.75.0");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        chunk = curl_slist_append(chunk, "Content-Type: application/json");
        if (authed) {
            std::string time_stamp = GetTimestamp();
            std::string sign = auth.Sign(time_stamp, method, path, body);
            chunk = curl_slist_append(chunk, ("CB-ACCESS-KEY: " + auth.Key).c_str());
            chunk = curl_slist_append(chunk, ("CB-ACCESS-SIGN: " + sign).c_str());
            chunk = curl_slist_append(chunk, ("CB-ACCESS-TIMESTAMP: " + time_stamp).c_str());
            chunk = curl_slist_append(chunk, ("CB-ACCESS-PASSPHRASE: " + auth.Passphrase).c_str());
        }
        res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        if (method == "POST") {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, -1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        }
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        /* Check for errors */
        if (res != CURLE_OK)
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;

        /* always cleanup */
        curl_easy_cleanup(curl);
        /* free the custom headers */
        curl_slist_free_all(chunk);
    }
    return readBuffer;
}

std::string Api::call(const std::string &method, bool authed, const std::string &path) {
    return call(method, authed, path, "");
}

std::string Api::GetTimestamp() {
    time_t t = time(nullptr);
    return std::to_string(t);
}
