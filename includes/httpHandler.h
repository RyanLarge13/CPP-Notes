#include <curl/curl.h>

#include <iostream>

#include "./colors.h"

using namespace std;

// Using curl for http request handling

#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H
class HttpHandler {
private:
  CURL *curl = nullptr;
  string baseUrl = "https://notesserver-production-9640.up.railway.app";

public:
  HttpHandler() {
    CURLcode result = curl_global_init(CURL_GLOBAL_DEFAULT);

    if (result != CURLE_OK) {
      string errorStr =
          "Failed to initialize libcurl: " + curl_easy_strerror(result);
      throw runtime_error(errorStr);

      return;
    }

    curl = curl_easy_init();

    if (!curl) {
      string errorStr = "Failed to initialize curl handle";
      throw runtime_error(errorStr);
    }
  }

  ~HttpHandler() {
    if (curl) {
      curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
  }

  auto setAndSendCurlCall(const string &url, auto &headers,
                          const string &jsonData) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());

    auto res = curl_easy_perform(curl);

    return res;
  }

  bool saveNote(const string &note, const string &title, int folderId,
                bool locked) {
    string url = baseUrl + "/notes/create";
    string lockedString = locked ? "true" : "false";

    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, "Content-Type: application/json");

    string jsonData = "{\"htmlNotes\": \"" + note + "\", \"title\": \"" +
                      title + ", \"folderId\": " + to_string(folderId) +
                      ", \"locked\": \"" + lockedString + "\"}";

    res = setAndSendCurlCall(url, headers, jsonData);

    if (res != CURLE_OK) {
      cout << RED + "Network request failed: " + ENDCOLOR << endl;
      cout << stderr << endl;
      cout << "Response: " << endl << curl_easy_strerror(res) << endl;
      return false;
    }

    return true;
  }

  void login(const string &username, const string &email,
             const string &password) {
    string url = baseUrl + "/user/login";
    struct curl_slist *headers = NULL;

    headers = curl_slist_append(headers, "Content-Type: application/json");

    string jsonData = "{\"username\": \"" + username + "\", \"email\": \"" +
                      email + ", \"password\": " + password "}";

    res = setAndSendCurlCall(url, headers, jsonData);

    if (res != CURLE_OK) {
      cout << RED + "Network request failed: " + ENDCOLOR << endl;
      cout << stderr << endl;
      cout << "Response: " << endl << curl_easy_strerror(res) << endl;
      return false;
    }

    return true;
  }
};

#endif