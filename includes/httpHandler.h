#include <curl/curl.h>
#include <iostream>
#include "./colors.h"
using namespace std;

// Using curl for http request handling

#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H
class HttpHandler {
    private:
    public:
    bool saveNote(const string& note, const string& title, const int& folderId, const bool& locked, const string& url) {
        CURL 8curl;
        CURLcode res;
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if (curl) {
            struct curl_slist *headers = NULL;
            headers = curl_slist_append(headers, "Content-Type: application/json");
            string jsonData = "{\"htmlNotes\": \"" + note + "\", \"title\": \"" + title + ", \"folderId\": " + to_string(folderId) + ", \"locked\": \"" + locked ? "true" : "false" + "\"}";
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURL_POSTFIELDS, jsonData.c_str());
            res = curl_easy_perform(curl);
            if (res != CURLE_OK) {
                cout << RED + "Network request failed: " + ENDLCOLOR << endl;
                cout << stderr << endl;
                cout << "Response: " << endl << curl_easy_strerror(res) << endl;
            }
            curl_easy_cleanup(curl);
        }
        curl_global_cleanup();
    }
};

#endif