#include <iostream>
#include <limits>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

class ConfigManager {
private:

   string eraseWhiteSpace(string value) {
      value.erase(value.begin(), find_if(value.begin(), value.end(), [](int ch) {
         return !isspace(ch);
         }));
      value.erase(find_if(value.rbegin(), value.rend(), [](int ch) {
         return !isspace(ch);
         }).base(), value.end());
      return value;
   }

   void changeLogin(string state) {
      ofstream file("config.yaml");
      if (!file.is_open()) {
         cout << "We could not open your configuration file. Please try again" <<
            endl;
         return;
      }
      vector < string > rows = getUserInfo(true);
      if (rows.empty()) {
         cout << "No rows returned" << endl;
         file.close();
         return;
      }
      rows[0] = "logged_in: " + state + "\n";
      for (const string& row : rows) {
         file << row;
      }
      file.close();
   }

   void logginFirst() {
      system("clear");
      int pin;
      bool input = true;
      cout << "You are logged out" << endl;
      cout << "Login with your pin: ";
      while (input) {
         cin >> pin;
         if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits < streamsize > ::max());
            cout << "Please input your 4 digit numeric pin" << endl;
            cout << "Login with your pin: ";
         }
         vector < string > rows = getUserInfo(false);
         if (stoi(rows[3]) == pin) {
            system("clear");
            changeLogin("true");
            input = false;
         }
         else {
            cout << endl << "Incorrect pin. Please try again" << endl;
            cout << "Login with your pin: ";
         }
      }
   }

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
            cin.ignore(numeric_limits < streamsize > ::max());
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
      if (username.size() < 1 || email.size() < 1 || password.size() < 1 || pin < 1000) {
         cout << "You must complete the registration process before creating a new account" << endl;
         return false;
      }
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
      conifigFile << "logged_in: " << "true" << "\n";
      conifigFile << "username: " << username << "\n";
      conifigFile << "email: " << email << "\n";
      conifigFile << "password: " << password << "\n";
      conifigFile << "pin: " << pin << "\n";
      return true;
   }

public:

   vector < string > getUserInfo(bool rawData) {
      ifstream file("config.yaml");
      if (!file.is_open()) {
         cout << "could not open file" << endl;
         return {};
      }
      string line;
      string value;
      vector < string > rows;
      // file.seekg(0, ios::end);
      // if (file.tellg() == 0) {
      //    file.close();
      //    cout << "It seems as though you have not finished creating your account, lets set that up" << endl;
      //    finishCreatingAccount(rows);
      //    return {};
      // }
      while (getline(file, value)) {
         if (rawData) {
            cout << "raw data " << value << endl;
            rows.push_back(value);
         }
         if (!rawData) {
            cout << "Formatted data " << endl;
            size_t colonPosition = value.find(":");
            if (colonPosition != string::npos) {
               string lineValue = value.substr(colonPosition + 1);
               string formattedLineValue = eraseWhiteSpace(lineValue);
               rows.push_back(formattedLineValue);
            }
         }
      }
      if (file.fail() && !file.eof()) {
         cout << "There was a problem reading your configuration file" << endl << "Would you like to attempt creating a new configuration or fix the issue manually? (Y/n) ";
      }
      file.close();
      cout << "Got user info" << endl;
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

   int checkForExistingAccount() {
      ifstream file("config.yaml");
      bool confirmed = false;
      if (!file.is_open()) {
         cout << "There was an error creating or opening a new configuration file for your account." << endl << "Would you like to manually go through the steps to fix this problem? (Y/n)" << endl;
         string confirmation;
         while (!confirmed) {
            cin >> confirmation;
            if (cin.fail()) {
               cin.clear();
               cin.ignore(numeric_limits < streamsize > ::max(), '\n');
               confirmed = true;
               return 1;
            }
            if (confirmation != "Y" || confirmation != "y") {
               cout << endl << "Steps to take: " << endl << "1. End this program and within your current directory type in \"touch config.yaml\" " << endl << "2. Run a command to make sure that you have read write and execution access within the directory \"chmod 777\"" << endl << "3. You are all set. Restart the application and try again";
               confirmed = true;
               return 1;
            }
         }
      }
      vector < string > rows = getUserInfo(false);
      if (file.fail() && !file.eof()) {
         cout << "There was a problem reading your configuration file" << endl << "Would you like to attempt creating a new configuration or fix the issue manually?" << endl;
         return 1;
      }
      file.close();
      if (rows.size() < 5) {
         string answer;
         bool getAnswer = true;
         cout << "It seems as though you have started creating an account but never finished the process. Would you like to continue?" << endl;
         while (getAnswer) {
            cin >> answer;
            if (cin.fail()) {
               cin.clear();
               cin.ignore(numeric_limits < streamsize > ::max());
               cout << "Please input a valid answer" << endl;
            }
            if (answer == "y" || answer == "Y") {
               finishCreatingAccount(rows);
               getAnswer = false;
               return 0;
            }
            else {
               getAnswer = false;
               return 1;
            }
         }
      }
      if (rows.size() == 5) {
         string loggedInLine = rows[0];
         if (loggedInLine == "true") {
            return 0;
         }
         if (loggedInLine == "false") {
            logginFirst();
            return 0;
         }
      }
      return 1;
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

   void finishCreatingAccount(vector < string > accountInfo) {
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

   void logout() {
      changeLogin("false");
      return;
   }

   void deleteAccount() {
      remove("config.yaml");
      return;
   }
};

#endif