#include <iostream>
using namespace std;

#ifndef IO_HAMDLER_H
#define IO_HAMDLER_H

class IoHandler {
 private:
 public:
 bool getInput() {} 
 string getInput(vector <string>& texts, const string& question) {
    bool input = true;
    string answer;
    for (const string& text : texts) {
        cout << text << endl;
    }
    cout << question;
    while (input) {
        cin >> answer;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits < streamsize > ::max());
            cout << endl << "Please select a valid option" << endl;
        }
        input = false;
        return answer;
    }
 } 
 int getInput(vector <string>& texts, const string& question) {
    bool input = true;
    int answer;
      for (const string& text : texts) {
        cout << text << endl;
    }
    cout << question;
     while (input) {
        cin >> answer;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits < streamsize > ::max());
            cout << endl << "Please select a valid option" << endl;
        }
        input = false;
        return answer;
    }
 } 
};

#endif