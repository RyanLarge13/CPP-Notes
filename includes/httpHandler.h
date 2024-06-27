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
  string lockedString = locked ? "true": "false";
  CURL *curl;
  CURLcode res;
  // global init will need to happen in the constructor
  curl_global_init(CURL_GLOBAL_DEFAULT);
  // curl_global_cleanup call cleanup when exiting the program
  // global init will need to happen in the constructor
  curl = curl_easy_init();
  if (curl) {
   struct curl_slist *headers = NULL;
   headers = curl_slist_append(headers, "Content-Type: application/json");
   string jsonData = "{\"htmlNotes\": \"" + note + "\", \"title\": \"" + title +
   ", \"folderId\": " + to_string(folderId) + ", \"locked\": \"" + lockedString + "\"}";
   curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
   curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonData.c_str());
   // use this for debugging CURLOPT_VERBOSE
   // If you at any point would like to blank all previously set options for a single easy handle, you can call
   // curl_easy_reset and you can also make a clone of an easy handle (with all its set options) using curl_easy_duphandle.
   res = curl_easy_perform(curl);
   if (res != CURLE_OK) {
    cout << RED + "Network request failed: " + ENDCOLOR << endl;
    cout << stderr << endl;
    cout << "Response: " << endl << curl_easy_strerror(res) << endl;
    return false;
   }
   // call reset if needed to clean up opts curl_easy_reset();
   // skip cleanup to continue using the handle
   curl_easy_cleanup(curl);
   return true;
  }
  curl_global_cleanup();
  return false;
 }
};

#endif