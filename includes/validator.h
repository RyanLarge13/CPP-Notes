#ifndef VALIDATOR_H
#define VALIDATOR_H

using namespace std;

class Validator {
 private:
 public:
 bool checkValidString(const int &minLength, const int &maxLength,
  const vector < char > &excludeCharacters,
  const string &stringToValidate) {
  const int length = stringToValidate.size();
  if (length < minLength || length > maxLength) {
   return false;
  }
  for (int i = 0; i < excludeCharacters.size(); i++) {
   if (stringToValidate.find(excludeCharacters[i]) != string::npos) {
    string error = "You cannot include '";
    error += excludeCharacters[i];
    error += "' characters in your new username";
    exceptionHandler.printPlainError(error);
    return false;
   }
  }
  return true;
 }

 bool checkValPin(const int& pin,
  const int &minSize,
  const int &maxSize,
  const int &length) {
  if (pin > maxSize || pin < minSize) {
   return false;
  }
  int const stringPinLength = to_string(pin).size();
  if (stringPinLength > length || stringPinLength < length) {
   return false;
  }
  return true;
 }
};

#endif