#ifndef VALIDATOR_H
#define VALIDATOR_H

using namespace std;

class Validator {
private:
public:
  bool checkValidString(const int &minLength, const int &maxLength,
                        const vector<char> &excludeCharacters,
                        const string &stringToValidate) {
    const int length = stringToValidate.size();
    if (length < minLength || length > maxLength) {
      return false;
    }
    for (int i = 0; i < excludeCharacters.length; i++) {
      if (stringToValidate.find(excludeCharacters[i]) == string::npos) {
        return false;
      }
    }
    return true;
  }

  bool checkValPin(int pin, const int &minSize, const int &maxSize,
                   const int &length) {
    if (pin > maxSize || pin < minSize) {
      return false;
    }
    int const stringPinLength = to_string(pin).size();
    if (stringPin > length || stringPin < length) {
      return false;
    }
    return true;
  }
};

#endif