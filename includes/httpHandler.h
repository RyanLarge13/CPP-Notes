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
  curl_slist *jsonHeaders;
  string baseUrl = "https://notesserver-production-9640.up.railway.app";

  CURLcode setAndSendCurlCall(const string &url, const string &jsonData) {
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, jsonHeaders);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());

    CURLcode res = curl_easy_perform(curl);

    return res;
  }

public:
  // JSON values capsule
  struct j {
    string key;
    variant<string, int, bool> value;
    jsonValues(const string &key, const variant<string, int, bool> &value)
        : key(key), value(value) {}
  }

  HttpHandler() {
    CURLcode result = curl_global_init(CURL_GLOBAL_DEFAULT);
    jsonHeaders = curl_slist_append(nullptr, "Content-Type: application/json");

    if (result != CURLE_OK) {
      cout << curl_easy_strerror(result) << endl;
      throw runtime_error("Failed to initialize libcurl. Check httpHandler");

      return;
    }

    curl = curl_easy_init();

    if (!curl) {
      throw runtime_error("Failed to initialize curl handle");
    }
  }

  ~HttpHandler() {
    curl_slist_free_all(jsonHeaders);

    if (curl) {
      curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
  }

  string serializeJsonString(const string &jsonString) {
    string newString = "";
    for (char ch : jsonString) {
      switch (ch) {
      case '"':
        newString += '\\';
        newString += ch;
        break;
      case '\\':
        newString += '\\';
        newString += ch;
        break;
      case '\n':
        newString += "\\n";
        break;
      case '\t':
        newString += "\\t";
        break;
      case '\r':
        newString += "\\r";
        break;
      default:
        newString += ch;
        break;
      }
    }

    return newString;
  }

  string buildJson(const vector<j> &json) {
    string jsonString = "{";

    for (const j &keyValue : json) {
      string key = keyValue.key;
      variant<string, int, bool> value = keyValue.value;

      jsonString += "\"" + serializeJsonString(key) + "\": ";

      if (holds_alternative<string>(value)) {
        jsonString += "\"" + serializeJsonString(get<string>(value)) + "\","
      }
      if (holds_alternative<int>(value)) {
        jsonString += to_string(get<int>(value)) + ",";
      }
      if (holds_alternative<bool>(value)) {
        string boolString = get<bool>(value) ? "true" : "false";
        jsonString += boolString + ",";
      }
    }

    // Remove final comma. Probably should check to make sure there is one or if
    // jsonString is empty
    jsonString.pop_back();
    jsonString += "}";

    return jsonString;
  }

  bool saveNote(const string &note, const string &title, int folderId,
                bool locked) {
    string url = baseUrl + "/notes/create";
    string lockedString = locked ? "true" : "false";

    vector<j> json = {j("htmlNotes" : note), j("title", title),
                      j("folderId", folderId), j("locked", locked)};
    string jsonData = buildJson(json);

    CURLcode res = setAndSendCurlCall(url, jsonData);

    if (res != CURLE_OK) {
      cout << RED + "Network request failed: " + ENDCOLOR << endl;
      cout << stderr << endl;
      cout << "Response: " << endl << curl_easy_strerror(res) << endl;
      return false;
    }

    return true;
  }

  bool login(const string &username, const string &email,
             const string &password) {
    string url = baseUrl + "/user/login";

    string jsonData = "{\"username\": \"" + username + "\", \"email\": \"" +
                      email + ", \"password\": " + password + "\}";

    CURLcode res = setAndSendCurlCall(url, jsonData);

    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

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