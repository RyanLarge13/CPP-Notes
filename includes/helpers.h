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

  // NOTE: Encrypt Decrypt table overview
  /**
  ENCRYPT

    vector<unsigned char>
            ↓
        encryptData()
            ↓
    EncryptedData
    ├── bytes
    └── nonce


  DECRYPT

    EncryptedData
    ├── bytes
    └── nonce
            ↓
        decryptData()
            ↓
    vector<unsigned char>
  */

  using EncryptionKey = std::array<unsigned char, crypto_secretbox_KEYBYTES>;
  using Nonce = std::array<unsigned char, crypto_secretbox_NONCEBYTES>;

  struct EncryptedData {
    vector<unsigned char> bytes;
    Nonce nonce;

    EncryptedData(const vector<unsigned char> &bytes, const Nonce &nonce)
        : bytes(bytes), nonce(nonce) {}
  };

  bool didSodiumStart() {
    if (sodium_init() < 0) {
      return false;
    }
    return true;
  }

  static string
  encryptedBytesToBase64String(const vector<unsigned char> &encryptedBytes) {
    char base64Encoded[sodium_base64_ENCODED_LEN(
        encryptedBytes.size(), sodium_base64_VARIANT_ORIGINAL)];

    sodium_bin2base64(base64Encoded, sizeof base64Encoded,
                      encryptedBytes.data(), encryptedBytes.size(),
                      sodium_base64_VARIANT_ORIGINAL);

    return base64Encoded;
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

    return base64EncodedKey;
  }

  static EncryptionKey
  getEncriptionKeyAsBytes(const string &base64EncodedString) {
    EncryptionKey key;

    size_t decodedLen;

    int result =
        sodium_base642bin(key.data(), key.size(), base64EncodedString.c_str(),
                          base64EncodedString.size(), nullptr, &decodedLen,
                          nullptr, sodium_base64_VARIANT_ORIGINAL);

    if (result != 0) {
      // NOTE: Failed to decode string
      // TODO: Must eventually make sure the application can remove this key log
      // the user out and start over
      throw runtime_error("Error transorming base64 encoded string into the "
                          "raw encryption key");
    }

    if (decodedLen != crypto_secretbox_KEYBYTES) {
      // NOTE: Data is not the same size as what is to be expected
      // TODO: Must eventually make sure the application can remove this key log
      // the user out and start over
      throw runtime_error("Error checking to see if the key has the desired "
                          "length as expected");
    }

    return key;
  }

  static EncryptedData encryptData(const vector<unsigned char> &bytes,
                                   const string &base64EncodedString) {
    EncryptionKey key = getEncriptionKeyAsBytes(base64EncodedString);

    Nonce nonce;

    randombytes_buf(nonce.data(), nonce.size());

    vector<unsigned char> encryptedBytes(bytes.size() +
                                         crypto_secretbox_MACBYTES);

    crypto_secretbox_easy(encryptedBytes.data(), bytes.data(), bytes.size(),
                          nonce.data(), key.data());

    return EncryptedData(encryptedBytes, nonce);
  }

  static vector<unsigned char> decryptData(const EncryptedData &encryptedData,
                                           const string &base64EncodedString) {
    EncryptionKey key = getEncriptionKeyAsBytes(base64EncodedString);

    if (encryptedData.bytes.size() < crypto_secretbox_MACBYTES) {
      // NOTE: Failed to decode string
      // TODO: Must eventually make sure the application can remove this key log
      // the user out and start over
      throw runtime_error("Invalid encrypted data");
    }

    vector<unsigned char> decryptedBytes(encryptedData.bytes.size() -
                                         crypto_secretbox_MACBYTES);

    int result = crypto_secretbox_open_easy(
        decryptedBytes.data(), encryptedData.bytes.data(),
        encryptedData.bytes.size(), encryptedData.nonce.data(), key.data());

    if (result != 0) {
      // NOTE: Data is not the same size as what is to be expected
      // TODO: Must eventually make sure the application can remove this key log
      // the user out and start over
      throw runtime_error("Failed to decrypt data");
    }

    return decryptedBytes;
  }
};

#endif