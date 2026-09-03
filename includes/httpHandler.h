#include "../common/exceptionHandlerInstance.h"
#include "../common/ioHandlerInstance.h"
#include <curl/curl.h>

#include <iostream>
#include <nlohmann/json.hpp>
#include <variant>

#include "./colors.h"

using namespace std;
using json = nlohmann::json;

// Using curl for http request handling

#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H
class HttpHandler {
public:
  struct HttpResponse {
    CURLcode curlCode;
    long httpCode;
    variant<json, string> body;

    HttpResponse(const CURLcode &curlCode, const long &httpCode,
                 const variant<string, json> &body)
        : curlCode(curlCode), httpCode(httpCode), body(body) {}
  };

  void printHttpError(long httpCode) {
    switch (httpCode) {
    case 400:
      exceptionHandler.printPlainError(
          YELLOW + "Server returned a bad reqeust code: " + httpCode +
          "status code" + ENDCOLOR +
          ". Please make sure you are sending valid data");
      break;
    case 401:
      exceptionHandler.printPlainError(
          YELLOW + "Server returned an unauthroied code: " + httpCode +
          "status code" + ENDCOLOR +
          ". Check your credentials. Try to login again");
      break;
    case 404:
      exceptionHandler.printPlainError(
          YELLOW + "Server returned a bad reqeust code: " + httpCode +
          "status code" + ENDCOLOR +
          ". Please make sure you are sending all of the information and valid "
          "information to complete this request");
      break;
    case 500:
      exceptionHandler.printPlainError(
          YELLOW + "Server returned a server error code: " + httpCode +
          "status code" + ENDCOLOR + ". Contact developer at" + BLUE +
          "ryanlarge@ryanlarge.dev" + ENDCOLOR);
      break;
    default:
      exceptionHandler.printPlainError(YELLOW + "Server returned a " +
                                       httpCode + "status code" + ENDCOLOR);
      break;
    }
  }

  // Return true if server call did not succeed or server returned some kind of
  // bad http request/failure
  bool handleCurlOrHttpCodeInformation(CURLcode curlCode, long httpCode) {
    if (curlCode != CURL_OK) {
      throw runtime_error("Curl failed to call to the server. Inside "
                          "handleCurlOrHttpCodeInformation. Check to see what "
                          "api call was last ran");
    }

    if (httpCode > 399) {
      printHttpError(httpCode);
      return true;
    }

    return false;
  }

private:
  CURL *curl = nullptr;
  curl_slist *jsonHeaders = nullptr;
  string baseUrl = "https://notesserver-production-9640.up.railway.app";

  static size_t writeCallback(char *contents, size_t size, size_t nmemb,
                              void *userData) {
    size_t totalSize = size * nmemb;

    string *response = static_cast<string *>(userData);
    response->append(contents, totalSize);

    return totalSize;
  }

  struct JsonData {
    string jsonData;
    bool hasJson;

    JsonData(const string &jsonData, bool hasJson)
        : jsonData(jsonData), hasJson(hasJson) {}
  };

  HttpResponse callAPI(const string &url, const JsonData &jsonObj,
                       const string &httpMethod) {
    curl_easy_reset(curl);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, jsonHeaders);
    // explicetly set http method type
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, httpMethod.c_str());

    if (jsonObj.hasJson) {
      curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonObj.jsonData.c_str());
    }

    // Response body
    string responseBody = "";
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseBody);

    // Curl OK or not
    CURLcode curlStatus = curl_easy_perform(curl);

    // Http status code
    long httpCode = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);

    try {
      josn parsedBody = json::parse(responseBody);
      HttpResponse res = HttpResponse(curlStatus, httpCode, parsedBody);

      return res;
    } catch (json::parse_error err) {
      HttpResponse res = HttpResponse(curlStatus, httpCode, responseBody);

      return res;
    }
  }

public:
  // JSON values capsule
  struct j {
    string key;
    variant<string, int, bool> value;
    j(const string &key, const variant<string, int, bool> &value)
        : key(key), value(value) {}
  };

  HttpHandler() {
    CURLcode result = curl_global_init(CURL_GLOBAL_DEFAULT);

    if (result != CURLE_OK) {
      cout << curl_easy_strerror(result) << endl;
      throw runtime_error("Failed to initialize libcurl. Check httpHandler");
    }

    curl = curl_easy_init();

    if (!curl) {
      // Must clean up here. Destructor wont run if construction throws
      curl_global_cleanup();

      throw runtime_error("Failed to initialize CURL easy handle");
    }

    jsonHeaders = curl_slist_append(nullptr, "Content-Type: application/json");

    if (!jsonHeaders) {
      // Clean up again because header construction can also fail
      curl_easy_cleanup(curl);
      curl = nullptr;

      curl_global_cleanup();

      throw runtime_error("Failed to create CURL JSON headers");
    }
  }

  // No copy construction possible for class
  HttpHandler(const HttpHandler &) = delete;
  // No copy assigning either of class
  HttpHandler &operator=(const HttpHandler &) = delete;

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
      const string &key = keyValue.key;
      const variant<string, int, bool> &value = keyValue.value;

      jsonString += "\"" + serializeJsonString(key) + "\": ";

      if (holds_alternative<string>(value)) {
        jsonString += "\"" + serializeJsonString(get<string>(value)) + "\",";
      } else if (holds_alternative<int>(value)) {
        jsonString += to_string(get<int>(value)) + ",";
      } else if (holds_alternative<bool>(value)) {
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

  HttpResponse saveNote(const string &note, const string &title, int folderId,
                        bool locked) {
    string url = baseUrl + "/notes/create";

    vector<j> json = {j("htmlNotes", note), j("title", title),
                      j("folderId", folderId), j("locked", locked)};
    string jsonData = buildJson(json);

    HttpResponse res = callAPI(url, JsonData(jsonData, true), "POST");

    return res;
  }

  HttpResponse login(const string &username, const string &email,
                     const string &password) {
    string url = baseUrl + "/users/login";

    vector<j> json = {j("username", username), j("email", email),
                      j("password", password)};
    string jsonData = buildJson(json);

    HttpResponse res = callAPI(url, JsonData(jsonData, true), "POST");

    return res;
  }
};

HttpResponse getUserData(const string &token) {
  string url = baseUrl + "/users/data";
  HttpResponse res = callAPI(url, JsonData("", false), "GET");
  return res;
}

#endif