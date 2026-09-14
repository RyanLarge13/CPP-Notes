#include <algorithm>
#include <cctype>
#include <iostream>
#include <sodium.h>

#include "../common/helpersInstance.h"

using namespace std;
using json = nlohmann::json;

#ifndef HELPERS_H
#define HELPERS_H

class Helpers {
public:
  // USAGE: Use to make ensure a good response incase the user typos when
  // attempting to type yes into the terminal

  // NOTE: Checks are based on commonly known
  // typos when a person is attempting to type yes
  static bool inputStringIsYes(const string &input) {
    string normalized = input;

    transform(normalized.begin(), normalized.end(), normalized.begin(),
              [](unsigned char ch) { return tolower(ch); });

    return normalized == "y" || normalized == "ye" || normalized == "yes" ||
           normalized == "yess" || normalized == "yse" || normalized == "eys";
  }

  // USAGE: When a json object needs multiple key checks especially when data
  // comes from the server
  static bool containsAll(const vector<string> &strings, const json &data) {
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
  static string mainDirStringCleanup(const string &mainDir) {
    // NOTE: Possibly move out of function and let caller handle this part to
    // make this method more versitile with creating dirs
    if (mainDir.size() < 1) {
      return "cpp-notes";
    }

    string input = mainDir;

    // TODO: Call remove_if in the future to handle more than a single ch
    input.erase(remove_if(input.begin(), input.end(),
                          [](char c) {
                            return c == '/' || c == '\\' || c == '\"' ||
                                   c == '\'';
                          }),
                input.end());

    return input;
  }

  // NOTE: Create a safe dir/file/config value
  static string eraseWhiteSpace(string value) {
    value.erase(value.begin(), find_if(value.begin(), value.end(),
                                       [](int ch) { return !isspace(ch); }));
    value.erase(find_if(value.rbegin(), value.rend(),
                        [](int ch) { return !isspace(ch); })
                    .base(),
                value.end());
    return value;
  }

  // -----------------------------------------------------------------------------
  // STRING ENCRYPTION AND DECRYPTION METHODS
  // ----------------------------------------------------------------------------
  bool didSodiumStart() {
    if (sodium_init() < 0) {
      return false;
    }
    return true;
  }

  static string genEncryptionKeyAsBase64() {
    unsigned char key[crypto_secretbox_KEYBYTES];

    crypto_secretbox_keygen(key);

    // NOTE: Explain how large the string will need to be based on how we
    // generate the key
    char base64EncodedKey[sodium_base64_ENCODED_LEN(
        crypto_secretbox_KEYBYTES, sodium_base64_VARIANT_ORIGINAL)];

    // NOTE: Encode base64EncodedKey! We pass in the char array we want the
    // characters to be pushed into, the key we generated. The size of each. and
    // the way we want sodium to process them
    sodium_bin2base64(base64EncodedKey, sizeof base64EncodedKey, key,
                      sizeof key, sodium_base64_VARIANT_ORIGINAL);

    string keyStringBase64 = base64EncodedKey;

    return keyStringBase64;
  }

  static unsigned char getExcriptionAsBytes(const string &base64EncodedString) {
    sodium_base642bin();
  }
};

#endif