#include <iostream>
#include <limits>
#include <sstream>
#include <vector>
#include <algorithm>
#include "../common/ioHandlerInstance.h"
#include "../common/fileManagerInstance.h"
#include "../common/exceptionHandlerInstance.h"
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

 bool changeLogin(string state) {
  vector < string > rows = getUserInfo(true);
  fstream* file = fileManager.openFileReadWrite("config.yaml");
  if (rows.empty() || !file) {
   exceptionHandler.printPlainError(
    "We could not log you in. Please try again"
   );
   return false;
  }
  rows[0] = "logged_in: " + state;
  for (const string& row: rows) {
   *file << row << "\n";
  }
  file->close();
  delete file;
  return true;
 }

 bool login() {
  int pin = ioHandler.getInput < int > ({{
   "You are logged out"
  }}, "Login with your pin: ", "Your pin will be a 4 digit number");
  vector < string > rows = getUserInfo(false);
  if (stoi(rows[4]) == pin) {
   system("clear");
   bool loginSuccess = changeLogin("true");
   if (loginSuccess) {
    return true;
   }
   else {
    exceptionHandler.printPlainError(
     "There was a problem logging you in Please try again"
    );
    return false;
   }
  }
  else {
   exceptionHandler.printPlainError(
    "Incorrect pin. Please try again"
   );
   return login();
  }
 }

 string createUsername() {
  string username = ioHandler.getInput < string > ({{
   ""
  }}, "New username: ", "Your new username must be valid characters");
  if (username.length() < 3) {
   exceptionHandler.printPlainError("Your username must be at least 3 characters in length");
   return createUsername();
  }
  return username;
 };

 string createEmail() {
  string email = ioHandler.getInput < string > ({{
   ""
  }}, "New Email: ", "Please provide a valid email address");
  if (email.length() < 5) {
   exceptionHandler.printPlainError("This email is too short. Please provide a valid email");
   return createEmail();
  }
  return email;
 }

 string createPassword() {
  string password = ioHandler.getInput < string > ({
   {
    "\nWe strongly suggest a secure and strong password containing:"
   },
   {
    "- At least 8 characters *"
   },
   {
    "- upper and lowercase characters"
   },
   {
    "- numbers"
   },
   {
    "- and special characters\n"
   }
  }, "New Password: ", "Please create a valid password");
  if (password.length() < 8) {
   exceptionHandler.printPlainError("Your password must be at least 8 characters in length");
   return createPassword();
  }
  return password;
 };

 int createPin() {
  int pin = ioHandler.getInput < int > ({{
   ""
  }}, "New 4-digit pin: ", "Your new pin must be a 4-digit number");
  if (pin > 9999 || pin < 1000) {
   cout << "Your new pin must be a 4-digit number" << endl;
   return createPin();
  }
  return pin;
 };

 bool nameMainDir(const string& dirname) {
  bool newDirCreated = fileManager.createNewDir("/" + dirname);
  if (!newDirCreated) {
   exceptionHandler.printPlainError("Make sure you have permissions set to rwx in your root directory in order for us to create your account");
   return false;
  }
  return true;
 }

 bool initializeConfig(const string& username, const string& email, string password, int pin,
  const string& mainDir, ofstream* configFile) {
  if (username.size() < 1 || email.size() < 1 || password.size() < 1 || pin < 1000) {
   exceptionHandler.printPlainError("You must complete the registration process before creating a new account");
   return false;
  }
  *configFile << "logged_in: " << "true" << "\n";
  *configFile << "username: " << username << "\n";
  *configFile << "email: " << email << "\n";
  *configFile << "password: " << password << "\n";
  *configFile << "pin: " << pin << "\n";
  *configFile << "maindir: " << mainDir << "\n";
  return true;
 }

 public:

 vector < string > getUserInfo(bool rawData) {
  fstream* file = fileManager.openFileReadWrite("config.yaml");
  string line;
  string value;
  vector < string > rows;
  while (getline(*file, value)) {
   if (rawData) {
    rows.push_back(value);
   }
   if (!rawData) {
    size_t colonPosition = value.find(":");
    if (colonPosition != string::npos) {
     string lineValue = value.substr(colonPosition + 1);
     string formattedLineValue = eraseWhiteSpace(lineValue);
     rows.push_back(formattedLineValue);
    }
   }
  }
  if (file->fail() && !file->eof()) {
   exceptionHandler.printInstructions(
    {
     {
      "Please reload the application and try again. We encountered an issue reading your configuration file."
     },
     {
      "1. Try deleting your configuration and re-running the program"
     },
     {
      "2. Make sure you have permissions set correctly to your file"
     }
    }
   );
   return {};
  }
  file->close();
  delete file;
  return rows;
 }

 ifstream* checkForLocalConfigFile(const string& fileName) {
  ifstream* fileExists = fileManager.checkExistingFile(fileName);
  if (!fileExists) {
   return nullptr;
  }
  return fileExists;
 }

 ofstream* createConfigFile(const string& fileName) {
  ofstream* newConfig = fileManager.createNewFile(fileName);
  if (!newConfig) {
   delete newConfig;
   bool userInput = exceptionHandler.handleError(
    {
     {
      "We are having issues initializing a configuration file"
     }
    },
    "Would you like to create this configuration file manually? (Y/n)"
   );
   if (!userInput) {
    return nullptr;
   }
   exceptionHandler.printInstructions(
    {
     {
      "Steps to take: "
     },
     {
      "1. End this program and within your current directory type in \"touch config.yaml\""
     },
     {
      "2. Run a command to make sure that you have read write and execution access within the directory \"chmod 777 config.yaml\""
     },
     {
      "3. You are all set, re-run the application and try again."
     }
    }
   );
   return nullptr;
  }
  return newConfig;
 }

 int checkForExistingAccount() {
  bool confirmed = false;
  vector < string > rows = getUserInfo(false);
  if (rows.size() == 0) {
   return 1;
  }
  if (rows.size() < 6) {
   exceptionHandler.handleError(
    {
     {
      "it looks like you have started creating an account but never finished."
     }
    },
    "Would you like to continue where you left off? (Y/n)"
   );
  }
  if (rows.size() == 6) {
   string loggedInLine = rows[0];
   if (loggedInLine == "true") {
    return 0;
   }
   if (loggedInLine == "false") {
    system("clear");
    bool loginSuccess = login();
    if (loginSuccess) {
     return 0;
    }
    else {
     return 1;
    }
   }
  }
  return 1;
 }

 void createAccount(ofstream* configFile) {
  cout << "Let create an account" << endl << "Welcome to CPP-Notes" << endl;
  cout << endl << "First we need to create a new username, email, password and a new pin incase you want to lock your notes" << endl;
  string newName = createUsername();
  string newEmail = createEmail();
  string newPassword = createPassword();
  int newPin = createPin();
  string mainDir = ioHandler.getInput < string > ({{
   "\nWe will be storing all of your notes at the root of your system in an accessible manner if you decide to manually update them."
  }}, "Main directory: ", "Please provide a valid name for this new directory");
  bool newMainDir = nameMainDir(mainDir);
  if (!newMainDir) {
   // Handle try again logic this is necessary
   return;
  }
  bool configInitialized = initializeConfig(newName, newEmail, newPassword,
   newPin, mainDir, configFile);
  if (configInitialized) {
   system("clear");
   cout << "\nWe officially created a new account for you. More configuration can be done within your settings. Happy notes taking!!!\n" << endl;
   return;
  }
  if (!configInitialized) {
   exceptionHandler.printInstructions(
    {
     {
      "Steps to take: "
     },
     {
      "1. End this program and within your current directory type in \"touch config.yaml\""
     },
     {
      "2. Run a command to make sure that you have read write and execution access within the directory \"chmod 777\""
     },
     {
      "3. You are all set. Restart the application and try again"
     }
    }
   );
   return;
  }
 }

 void finishCreatingAccount(vector < string > accountInfo) {
  size_t length = accountInfo.size();
  if (length < 1) {
   // createAccount();
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