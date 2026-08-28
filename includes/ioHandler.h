#include <iostream>
#include <limits>
#include <string>
#include <vector>
using namespace std;

#ifndef IO_HAMDLER_H
#define IO_HAMDLER_H

class IoHandler {
 private:
 public:
  template <typename T>
  T getInput(const vector<string>& texts, const string& question,
             const string& reply) {
    bool input = true;
    T answer;
    for (const string& text : texts) {
      cout << text << endl;
    }
    cout << question;
    while (input) {
      cin >> answer;
      if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl << reply << endl;
        input = false;
      }
      input = false;
    }
    return answer;
  }
};

#endif