#include <iostream>
#include <limits>
#include <string>
#include <variant>
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
      cout << text << endl << endl;
    }
    cout << question;
    while (input) {
      cin >> answer;
      if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl << reply << endl << endl;
        input = false;
      }
      input = false;
    }
    return answer;
  }

  variant<int, string> getInput(const vector<string>& texts,
                                const string& question) {
    string answer;

    for (const string& text : texts) cout << text << endl << endl;

    cout << question;
    getline(cin >> ws, answer);

    try {
      size_t pos;
      int number = stoi(answer, &pos);

      if (pos == answer.size()) return number;
    } catch (const std::exception& e) {
      return answer;
    }

    return answer;
  }
};

#endif