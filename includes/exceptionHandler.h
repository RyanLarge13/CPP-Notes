#include <iostream>
#include <limits>
#include <vector>

using namespace std;

#ifndef EXCEPTION_HANDLER_H
#define EXCEPTION_HANDLER_H

class ExceptionHandler {
 private:
 public:
  bool handleError(const vector<string>& messages, const string& question) {
    for (const string& message : messages) {
      cout << message << endl;
    }

    string answer;
    cout << question;
    cin >> answer;

    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max());
      return false;
    }

    if (answer == "Y" || answer == "y") {
      return true;
    } else {
      return false;
    }
  }

  void printPlainError(const string& error) { cout << error << endl << endl; }

  void printStringResBody(long httpCode, const string& body) {
    string errStr = "\\n\\n" + YELLOW + "HTTP status code: \\n" + ENDCOLOR +
                    to_string(httpCode) + "\\n\\n" + YELLOW + "Res body: \\n" +
                    ENDCOLOR + body + "\\n";

    printPlainError(errStr);
  }

  void printInstructions(const vector<string>& instructions) {
    for (const string& instruction : instructions) {
      cout << instruction << endl << endl;
    }
  }
};

#endif