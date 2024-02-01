#include <iostream>
#include <limits>
#include <sstream>
using namespace std;

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

class ConfigManager {
  private:
  string createUsername() {
    string username;
    bool getUsername = true;
    while(getUsername) {
      cout << "New username: ";
      cin << username;
      if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits < streamsize > ::max());
        cout << endl << "Your new username must be valid characters" << endl;
      } else if (username.length < 3) {
        cout << endl << "Your username must be at least 3 characters in length"
        << endl;
      } else {
        getUsername = false;
      }
    }
    return username;
  };
  vector < pair < string,
  bool>> createUsername() {};
  string createPassword() {
    string password;
    bool getPassword = true;
    while(getPassword) {
      cout << "We strongy suggest a secure and strong password that contains upper and lowercase characters, numbers, and special characters" << endl:
      cout << "New password: ";
      cin >> password;
      if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize >:ax());
        cout << "Please create a valid password" << endl;
      } else if (password.length < 8) {
        cout << "Your password must be at least 8 characters in length" << endl;
      } else {
        getPasswordConfirm = false;
      }
    }
  };
  int createPin() {};
  public:
  bool checkForLocalConfigFile() {
    string filename = "config.yaml";
    ifstream file(filename);
    if (!file.is_open()) {
      return false;
    }
    return true;
  }

  void createConfigFile(string filename) {
    ofstream newFile(filename);
    if (!newFile.is_open()) {
      char answer;
      cout << endl << "We are having issues initializing a configuration file for your new account. Would you like to create this configuration file manually?" << endl << " (Y/N) Yes/No: ";
      cin >> answer;
      if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits < streamsize > ::max());
        return;
      }
      if (answer == 'Y' || answer == 'y') {
        cout << endl << "Steps to take: " << endl << "1. End this program and within your current directory type in \"touch config.yaml\" " << endl << "2. Run a command to make sure that you have read write and execution access within the directory \"chmod 777 config.yaml\"" << endl << "3. You are all set. Restart the application and try again";
        return;
      }
    }
    return;
  }

  bool checkForExistingAccount() {
    ifstream file("config.yaml");
    bool confirmed = false;
    if (!file.is_open()) {
      cout << "There was an error creating or opening a new configuration file for your account." << endl << "Would you like to manually go through the steps to fix this problem?" << endl;
      string confirmation;
      while (!confirmed) {
        cin >> confirmation;
        if (cin.fail()) {
          cin.clear();
          cin.ignore(numeric_limits < streamsize > ::max(), '\n');
          confirmed = true;
          return false;
        }
        if (confirmation != "Y" || confirmation != "y") {
          cout << endl << "Steps to take: " << endl << "1. End this program and within your current directory type in \"touch config.yaml\" " << endl << "2. Run a command to make sure that you have read write and execution access within the directory \"chmod 777\"" << endl << "3. You are all set. Restart the application and try again";
          confirmed = true;
          return false;
        }
      }
    }
    string line;
    string value;
    getline(file, line);
    stringstream ss(line);
    vector < string > rows;
    while(getline(ss, value, '\n')) {
      rows.push_back(value);
    }
    for (const string& row: rows) {
      cout << row << endl;
    }
    if (rows.size() < 1) {
      return false;
    }
    return false;
  }

  void createAccount() {
    cout << "Let create an account" << endl << "Welcome to CPP-Notes" << endl;
    cout << endl << "First we need to create a new username, email, password and a new pin incase you want to lock your notes" << endl;
    string newName = createUsername();
    vector < pair < string,
    bool>> newEmail = createEmail();
    string newPassword = createPassword();
    int newPin = createPin();
  }
};

#endif