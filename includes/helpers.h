#include <algorithm>
#include <cctype>
#include <iostream>

using namespace std;
using json = nlohmann::json;

#ifndef HELPERS_H
#define HELPERS_H

class Helpers {

  // USAGE: Use to make ensure a good response incase the user typos when
  // attempting to type yes into the terminal

  // NOTE: Checks are based on commonly known
  // typos when a person is attempting to type yes
  inline static bool inputStringIsYes(const string &input) {
    string normalized = input;

    transform(normalized.begin(), normalized.end(), normalized.begin(),
              [](unsigned char ch) { return tolower(ch); });

    return normalized == "y" || normalized == "ye" || normalized == "yes" ||
           normalized == "yess" || normalized == "yse" || normalized == "eys";
  }

  // USAGE: When a json object needs multiple key checks especially when data
  // comes from the server
  inline static bool containsAll(const vector<string> &strings,
                                 const json &data) {
    for (int i = 0; i < strings.size(); i++) {
      if (!data.contains(strings[i])) {
        return false;
      }
    }
    return true;
  }

  // USAGE: When user is creating custom main dir name and forward slashes
  // and other strange characters need to be removed

  // WARNING: Do not use on other strings unless building custom main dir
  inline static string mainDirStringCleanup(const string &mainDir) {

    // NOTE: Possibly move out of function and let caller handle this part to
    // make this method more versitile with creating dirs
    if (mainDir.size() < 1) {
      return "cpp-notes";
    }

    string input = mainDir;

    // TODO: Call remove_if in the future to handle more than a single ch
    input.erase(remove(input.begin(), input.end(), '/'), input.end());

    return input;
  }

  // NOTE: Be careful. Not even sure if this can be used as all tables in DB for
  // this apps API might take integer id form eg auto increment
  inline static string generateLinuxUUID() {
    uuid_t uuid;
    uuid_generate(uuid); // Generates a random or time-based UUID based on
                         // system capability

    char out[37]; // 36 chars + null terminator
    uuid_unparse(uuid, out);

    return string(out);
  }
};