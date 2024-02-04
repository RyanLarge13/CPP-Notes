#include <iostream>
#include <limits>
#include <sstream>
#include <vector>
using namespace std;

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

class ConfigManager {
private:

  string createUsername() {
    string username;
    bool getUsername = true;
    while (getUsername) {
      cout << "New username: ";
      cin >> username;
      if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits < streamsize > ::max());
        cout << endl << "Your new username must be valid characters" << endl;
      }
      else if (username.length() < 3) {
        cout << endl << "Your username must be at least 3 characters in length"
          << endl;
      }
      else {
        getUsername = false;
      }
    }
    return username;
  };

  string createEmail() {
    string answer;
    string email;
    bool getEmail = true;
    bool getAnswer = true;
    cout << "You have a choice to add an email to your account or not. This will help with recovering information and storing information on the cloud when using other devices of recovering data" << endl;
    while (getAnswer) {
      cout << "Would you like to add an email?  (Y/n)";
      cin >> answer;
      if (cin.fail()) {
        getAnswer = false;
      }
      if (answer == "y" || answer == "Y" || answer == "yes" || answer == "Yes") {
        getAnswer = false;
        break;
      }
      else {
        getAnswer = false;
        cout << "No problem, you can change this later if you are interested" << endl;
        return "no email";
      }
      return "no email";
    }
    cout << "Okay, sounds good" << endl;
    while (getEmail) {
      cout << "New email: ";
      cin >> email;
      if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max());
        getEmail = false;
        return "no email";
      }
      if (email.length() < 5) {
        cout << "Please input a valid email" << endl;
      }
      else {
        getEmail = false;
      }
    }
    cout << endl << "Great!" << endl;
    return email;
  }

  string createPassword() {
    string password;
    bool getPassword = true;
    while (getPassword) {
      cout << "We strongly suggest a secure and strong password that contains upper and lowercase characters, numbers, and special characters" << endl;
      cout << "New password: ";
      cin >> password;
      if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits < streamsize > ::max());
        cout << "Please create a valid password" << endl;
      }
      else if (password.length() < 8) {
        cout << "Your password must be at least 8 characters in length" << endl;
      }
      else {
        getPassword = false;
        return password;
      }
    }
    return password;
  };

  int createPin() {
    int pin;
    bool getPin = true;
    while (getPin) {
      cout << "New 4-digit pin: ";
      cin >> pin;
      if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits < streamsize > ::max());
        cout << "Your new pin must be a 4-digit number" << endl;
      }
      if (pin > 9999 || pin < 1000) {
        cout << "Your new pin must be a 4-digit number" << endl;
      }
      else {
        getPin = false;
        return pin;
      }
    }
    return pin;
  };

  bool initializeConfig(string username, string email, string password, int pin) {
    ofstream conifigFile("config.yaml");
    if (!conifigFile.is_open()) {
      char answer;
      cout << endl << "We are having issues initializing a configuration file for your new account. Would you like to create this configuration file manually?" << endl << " (Y/N) Yes/No: ";
      cin >> answer;
      if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits < streamsize > ::max());
        return false;
      }
      if (answer == 'Y' || answer == 'y') {
        cout << endl << "Steps to take: " << endl << "1. End this program and within your current directory type in \"touch config.yaml\" " << endl << "2. Run a command to make sure that you have read write and execution access within the directory \"chmod 777 config.yaml\"" << endl << "3. You are all set. Restart the application and try again";
        return false;
      }
    }
    conifigFile << "username: " << username << "\n";
    conifigFile << "email: " << email << "\n";
    conifigFile << "password: " << password << "\n";
    conifigFile << "pin: " << pin << "\n";
    return true;
  }

public:

  vector <string> getUserInfo() {
    ifstream file("config.yaml");
    // if (!file.is_open()) {

    // }
    string line;
    string value;
    vector < string > rows;
    while (getline(file, value)) {
      size_t colonPosition = value.find(":");
      if (colonPosition != string::npos) {
        string lineValue = value.substr(colonPosition + 1);
        lineValue.erase(0, lineValue.find_first_not_of(" \t\n\r\f\v"));
        lineValue.erase(lineValue.find_last_not_of(" \t\n\r\f\v"));
        rows.push_back(lineValue);
      }
    }
    if (!file.eof()) {
      cout << "There was a problem reading your configuration file" << endl << "Would you like to attempt creating a new configuration or fix the issue manually?";
    }
    file.close();
    return rows;
  }

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
    vector < string > rows;
    while (getline(file, value)) {
      rows.push_back(value);
    }
    if (!file.eof()) {
      cout << "There was a problem reading your configuration file" << endl << "Would you like to attempt creating a new configuration or fix the issue manually?";
      return false;
    }
    file.close();
    if (rows.size() < 4) {
      string answer;
      bool getAnswer = true;
      cout << "It seems as though you have started creating an account but never finished the process. Would you like to continue?" << endl;
      while (getAnswer) {
        cin >> answer;
        if (cin.fail()) {
          cin.clear();
          cin.ignore(numeric_limits<streamsize>::max());
          cout << "Please input a valid answer" << endl;
        }
        if (answer == "y" || answer == "Y") {
          finishCreatingAccount(rows);
          getAnswer = false;
          return true;
        }
        else {
          getAnswer = false;
          return false;
        }
      }
    }
    return true;
  }

  void createAccount() {
    cout << "Let create an account" << endl << "Welcome to CPP-Notes" << endl;
    cout << endl << "First we need to create a new username, email, password and a new pin incase you want to lock your notes" << endl;
    string newName = createUsername();
    string newEmail = createEmail();
    string newPassword = createPassword();
    int newPin = createPin();
    bool configInitialized = initializeConfig(newName, newEmail, newPassword, newPin);
    if (configInitialized) {
      cout << "We officially created a new account for you. More configuration can be done within your settings. Happy notes taking!!!" << endl;
    }
    else {
      cout << endl << "Steps to take: " << endl << "1. End this program and within your current directory type in \"touch config.yaml\" " << endl << "2. Run a command to make sure that you have read write and execution access within the directory \"chmod 777\"" << endl << "3. You are all set. Restart the application and try again";
    }
  }

  void finishCreatingAccount(vector<string> accountInfo) {
    size_t length = accountInfo.size();
    if (length < 1) {
      createAccount();
      return;
    }
    if (length < 2) {
      cout << "Your current username is " << accountInfo[0] << ". Would you like to change it?";
      createEmail();
    }
    if (length < 3) {
      if (accountInfo[1] == "no email") {
        cout << "Your current username " << accountInfo[0] << " and you currently do not have an email. Would you like to change your username or email?";
      }
      else {
        cout << "Your current username " << accountInfo[0] << ", and your current email is " << accountInfo[1];
      }
      createPassword();
    }
    if (length < 4) {
      if (accountInfo[1] == "no email") {
        cout << "Your current username is " << accountInfo[0] << ", you do not have an email set, and your current password has been successfully set" << endl << "Would you like to change any of these values?";
      }
      else {
        cout << "Your current username is " << accountInfo[0] << ", your current email is " << accountInfo[1] << " and your current password has been successfully set" << endl << "Would you like to change any of these values?";
      }
      createPin();
    }
  }

};

#endif