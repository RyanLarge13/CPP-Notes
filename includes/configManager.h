#include <algorithm>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

#include "../common/exceptionHandlerInstance.h"
#include "../common/fileManagerInstance.h"
#include "../common/ioHandlerInstance.h"
#include "../common/validatorInstance.h"
#include "colors.h"

using namespace std;

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

class ConfigManager {
 private:
 void confirmPass(const string &password) {
  string confirmPassword = ioHandler.getInput < string > (
   {{
    "Confirm your password"
   }},
   "Confirm Password: ", "Please input valid characters");
  if (!validator.checkValidString(8, 20, {
   ';'
  }, confirmPassword)) {
   return;
  }
  if (confirmPassword == password) {
   return;
  } else {
   exceptionHandler.printPlainError(
    "Please type in your password again exactly the same as the first "
    "time");
   return confirmPass(password);
  }
 }

 string eraseWhiteSpace(string value) {
  value.erase(value.begin(), find_if(value.begin(), value.end(),
   [](int ch) {
    return !isspace(ch);
   }));
  value.erase(find_if(value.rbegin(), value.rend(),
   [](int ch) {
    return !isspace(ch);
   })
   .base(),
   value.end());
  return value;
 }

 bool changeLogin(string state) {
  if (state != "true" || state != "false") {
   return false;
  }
  vector < string > rows = getUserInfo(true);
  if (rows.empty()) {
   exceptionHandler.printPlainError(
    "We could not log you out. Please try again");
   return false;
  }
  fstream *file = fileManager.openFileReadWrite(fileManager.HOME_DIR + "/" +
   "config.yaml");
  if (!file) {
   return false;
  }
  rows[0] = "logged_in: " + state;
  for (const string &row: rows) {
   *file << row << "\n";
  }
  file->close();
  delete file;
  return true;
 }

 bool login() {
  int pin = ioHandler.getInput < int > (
   {{
    "You are logged out"
   }},
   "Login with your pin: ", "Your pin will be a 4 digit number");
  if (!validator.checkValPin(pin, 1111, 9999, 4)) {
   exceptionHandler.printPlainError("Please respond with a valid pin");
   exceptionHandler.printInstructions(
    {{
     "- Must be 4 digits", "- No less than 1111",
     "- No greater than 9999", "Please try again"
    }});
   return login();
  }
  vector < string > rows = getUserInfo(false);
  if (stoi(rows[4]) == pin) {
   system("clear");
   bool loginSuccess = changeLogin("true");
   if (loginSuccess) {
    return true;
   } else {
    exceptionHandler.printPlainError(
     "There was a problem logging you in Please try again");
    return login();
   }
  } else {
   exceptionHandler.printPlainError("Incorrect pin. Please try again");
   return login();
  }
 }

 string createUsername() {
  string username = ioHandler.getInput < string > (
   {{
    ""
   }}, "New username: ", "Your new username must be valid characters");
  if (!validator.checkValidString(
   3, 20,
   {
    '<', '>', ',', '{', '}', '[', ']', '!', '@', '#', '$', '%', '^',
    '&', '*', '(', ')', '+', '='
   },
   username)) {
   exceptionHandler.printPlainError("Please insert a valid username");
   exceptionHandler.printInstructions(
    {{
     "- Can ONLY contain:", "  - letters", "  - numbers",
     "  - underscores", "  - and dashes",
     "- Must be at least 3 characters long",
     "- Cannot be longer than 20 characters\n"
    }});
   return createUsername();
  }
  string confirm = ioHandler.getInput < string > (
   {{
    "Confirm " + RED + username + ENDCOLOR +
    " is the username you want"
   }},
   YELLOW + "(Y/n): " + ENDCOLOR,
   "Please give a valid answer, Y for yes n for no");
  if (confirm == "Y" || confirm == "y") {
   return username;
  } else {
   cout << endl
   << BLUE + "Please try again, or press Ctrl + c to exit the program" +
   ENDCOLOR
   << endl;
   return createUsername();
  }
  return username;
 };

 string createEmail() {
  string email = ioHandler.getInput < string > (
   {{
    ""
   }}, "New Email: ", "Please provide a valid email address");
  if (!validator.checkValidString(
   6, 50,
   {{
    '<', '>', ',', '{', '}', '[', ']', '!', '#', '$', '%', '^', '&',
    '*', '(', ')', '+', '='
   }},
   email)) {
   exceptionHandler.printPlainError("Please provide a valid email");
   exceptionHandler.printInstructions(
    {
     {
      "- Must be at least 6 characters",
      "- Cannot be longer than 50 characters",
      "- Cannot contain these special characters:",
      "'<', '>', ',', '.', '{', '}', '[', ']', '! , '#', '$', '%', '^', '&', '*', '(', ')', '+','='"
     }});
   return createEmail();
  }
  string confirm = ioHandler.getInput < string > (
   {{
    "Confirm " + RED + email + ENDCOLOR + " is the email you want"
   }},
   YELLOW + "(Y/n): " + ENDCOLOR,
   "Please give a valid answer, Y for yes, n for no");
  if (confirm == "Y" || confirm == "y") {
   return email;
  } else {
   cout << endl << BLUE + "Okay, try again" + ENDCOLOR << endl;
   return createEmail();
  }
  return email;
 }

 string createPassword() {
  string password = ioHandler.getInput < string > (
   {{
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
    }},
   "New Password: ", "Please create a valid password");
  if (!validator.checkValidString(8, 50, {
   ';'
  }, password)) {
   system("clear");
   exceptionHandler.printPlainError(
    "Please input a valid password, quit the program with Ctrl + c");
   return createPassword();
  }
  confirmPass(password);
  return password;
 };

 int createPin() {
  int pin = ioHandler.getInput < int > (
   {{
    ""
   }},
   "New 4-digit pin: ", "Your new pin must be a 4-digit number");
  if (!validator.checkValPin(pin, 1111, 9999, 4)) {
   exceptionHandler.printPlainError("Please input a valid pin");
   exceptionHandler.printInstructions(
    {{
     "- Must be 4 digits", "- Cannot be less then 1111",
     "- Cannot be greater than 9999",
     "Please try again. If you want to exit press Ctrl + c to quit "
     "at anytime"
    }});
   return createPin();
  }
  string confirm = ioHandler.getInput < string > (
   {{
    "Confirm " + RED + to_string(pin) + ENDCOLOR +
    " is what you want your new pin to be"
   }},
   YELLOW + "(Y/n): " + ENDCOLOR,
   "Please provide a valid response, Y for yes, n for no");
  if (confirm == "Y" || confirm == "y") {
   return pin;
  } else {
   system("clear");
   cout << BLUE + "Okay, try again" + ENDCOLOR << endl;
   return createPin();
  }
  return pin;
 };

 bool initializeConfig(const string &username, const string &email,
  string password, int pin, const string &mainDir,
  ofstream *configFile) {
  if (!configFile) {
   cout << "No config file defined canceling initialize config" << endl;
  }
  if (username.size() < 1 || email.size() < 1 || password.size() < 1 ||
   pin < 1000) {
   exceptionHandler.printPlainError("You must complete the registration "
    "process before creating a new "
    "account");
   configFile->close();
   return false;
  }
  *configFile << "logged_in: " << "true" << "\n";
  *configFile << "username: " << username << "\n";
  *configFile << "email: " << email << "\n";
  *configFile << "password: " << password << "\n";
  *configFile << "pin: " << pin << "\n";
  *configFile << "maindir: " << "/" + mainDir << "\n";
  configFile->close();
  return true;
 }

 string getNewUsername(const string &currentUsername) {
  const string newUsername = ioHandler.getInput < string > (
   {{
    "Type your current username again to cancel and return to the "
    "main "
    "menu"
   }},
   "\nNew username: ", "Please provide a valid new username");
  if (!validator.checkValidString(
   3, 20,
   {{
    '<', '>', ',', '{', '}', '[', ']', '!', '@', '#', '$', '%',
    '^', '&', '*', '(', ')', '+', '='
   }},
   newUsername)) {
   exceptionHandler.printPlainError("Please insert a valid username");
   exceptionHandler.printInstructions(
    {{
     "- Can ONLY contain:", "  - letters", "  - numbers",
     "  - underscores", "  - and dashes",
     "- Must be at least 3 characters long",
     "- Cannot be longer than 20 characters\n"
    }});
   return getNewUsername(currentUsername);
  }
  if (newUsername == currentUsername) {
   system("clear");
   cout << "Canceling" << endl;
   return "####";
  }
  const string confirmNewName =
  ioHandler.getInput < string > ({{
   ""
  }}, "Confirm your new username: ",
   "Please provide a valid confirmation");
  if (confirmNewName != newUsername) {
   cout << "Please try again" << endl;
   return getNewUsername(currentUsername);
  }
  return newUsername;
 }

 string getNewPass(const string &currentPass) {
  const string newPass = ioHandler.getInput < string > (
   {{
    ""
   }}, "New password: ", "Please provide a valid response");
  if (newPass == currentPass) {
   return "####";
  }
  // Check validation
  const string confirmPass = ioHandler.getInput < string > (
   {{
    ""
   }},
   "Confirm new password: ", "Please provide a valid response");
  if (confirmPass != newPass) {
   exceptionHandler.printPlainError(
    "Please type the exact same password. Try again");
   return getNewPass(currentPass);
  }
  return newPass;
 }

 int getNewPin(const int &currentPin) {
  const int newPin = ioHandler.getInput < int > (
   {{
    "Enter your original pin value to cancel and "
    "return to the main menu"
   }},
   "New pin: ", "Please provide a valid response");
  // Validate new pin
  if (newPin == currentPin) {
   return -999;
  }
  const int confirmNewPin = ioHandler.getInput < int > (
   {{
    ""
   }},
   "Confirm your new pin: ", "Please provide a valid response");
  if (confirmNewPin != newPin) {
   exceptionHandler.printPlainError(
    "You must confirm your new pin. Please try again");
   return getNewPin(currentPin);
  }
  return newPin;
 }

 bool updateConfig(const vector < string > &userInfo) {
  fstream *config = fileManager.openFileReadWrite(fileManager.HOME_DIR + "/config.yaml");
  if (!config) {
   delete config;
   return false;
  }
  *config << "logged_in: " << "true" << "\n";
  *config << "username: " << userInfo[1] << "\n";
  *config << "email: " << userInfo[2] << "\n";
  *config << "password: " << userInfo[3] << "\n";
  *config << "pin: " << userInfo[4] << "\n";
  *config << "maindir: " << "/" + userInfo[5] << "\n";
  config->close();
  delete config;
  return true;
 }

 public:
 bool changeUsername(vector < string > &userInfo) {
  cout << "Okay, let's change your username. To exit, simply type your "
  "current username when asked to give a new one"
  << endl;
  // If online account connected. Also change remote username.
  const string currentUsername = userInfo[1];
  const string confirmName = ioHandler.getInput < string > (
   {{
    ""
   }},
   "Confirm current username: ", "Please input a valid username");
  if (currentUsername != confirmName) {
   exceptionHandler.printPlainError("Try again");
   return changeUsername(userInfo);
  }
  string newName = getNewUsername(currentUsername);
  if (newName == "####") {
   // User typed same name again to exit and return to menu
   return false;
  }
  system("clear");
  userInfo[1] = newName;
  bool didUpdate = updateConfig(userInfo);
  if (!didUpdate) {
   userInfo[1] = currentUsername; exceptionHandler.printPlainError(
    "We could not update your local configuration.. We are terribly "
    "sorry. Try changing your username again.");
  }
  return didUpdate;
 }

 bool changePass(vector < string > &userInfo) {
  const string currentPass = userInfo[3];
  cout
  << "Okay, let's change your password. To cancel and return to main "
  "menu type in your current password again when asked for a new "
  "one"
  << endl;
  const string confirmPass = ioHandler.getInput < string > (
   {{
    ""
   }}, "Current Password: ", "Please provide a valid response");
  if (confirmPass != currentPass) {
   exceptionHandler.printPlainError(
    "You did not type in the correct password. Please try again");
   return changePass(userInfo);
  }
  string newPass = getNewPass(currentPass);
  if (newPass == "####") {
   return false;
  }
  userInfo[3] = newPass;
  bool didUpdate = updateConfig(userInfo);
  if (!didUpdate) {
   userInfo[3] = currentPass;
   exceptionHandler.printPlainError(
    "We could not update your local configuration.. We are terribly "
    "sorry. Try changing your password again.");
  }
  return didUpdate;
 }

 bool changePin(vector < string > &userInfo) {
  const int currentPin = stoi(userInfo[4]);
  cout << "Okay, sounds good. let's change your pin for logging in and "
  "opening locked notes"
  << endl;
  const int confirmPin = ioHandler.getInput < int > (
   {{
    ""
   }}, "Confirm your current pin: ", "Please provide a valid pin");
  // Validate pin
  if (currentPin != confirmPin) {
   exceptionHandler.printPlainError(
    "Please confirm the current pin you use to login");
   return changePin(userInfo);
  }
  int newPin = getNewPin(currentPin);
  if (newPin == -999) {
   return false;
  }
  userInfo[4] = newPin;
  bool configUpdated = updateConfig(userInfo);
  if (!configUpdated) {
   userInfo[4] = currentPin;
   exceptionHandler.printPlainError(
    "We could not update your local configuration.. We are terribly "
    "sorry. Try changing your pin again.");
  }
  return configUpdated;
 }

 bool changeDir(vector < string > &userInfo) {
  const string currentMainDir = userInfo[5];
  cout << "Okay, let's change the directory name that you store your "
  "folders "
  "in"
  << endl;
  const string confirmation = ioHandler.getInput < string > (
   {{
    ""
   }},
   "Are you sure you would like to change the "
   "name of your main directory? (Y/n): ",
   "Pleas provide a valid response");
  if (confirmation == "Y" || confirmation == "y") {
   const string newDirName = ioHandler.getInput < string > (
    {{
     ""
    }},
    "New directory name: ", "Please provide a valid response");
   // Validate new directory name
   const string confirmNewDirName = ioHandler.getInput < string > (
    {{
     ""
    }},
    "Confirm this is your new directory name: " + YELLOW +
    newDirName + ENDCOLOR,
    "Please provide a valid response");
   if (confirmNewDirName == "Y" || confirmNewDirName == "y") {
    cout << "Sounds good" << endl;
    const bool didUpdateDir =
    fileManager.renameDir(userInfo[5], newDirName);
    if (didUpdateDir) {
     userInfo[5] = newDirName;
     const bool didUpdateConfig = updateConfig(userInfo);
     if (!didUpdateConfig) {
      exceptionHandler.printPlainError(
       "We could not update your local configuration.. We are "
       "terribly "
       "sorry. Try changing your main directory again.");
      return false;
     }
     return true;
    }
    exceptionHandler.printPlainError(
     "We failed to update the directory name where we store your "
     "notes. "
     "Please double check to see if you have permissions set "
     "correctly "
     "for this directory form your home folder. Path: " +
     fileManager.getCurrentPath());
    return false;
   }
   return changeDir(userInfo);
  }
  return false;
 }

 bool nameMainDir(const string &dirname) {
  bool newDirCreated = fileManager.createNewDir(dirname);
  if (!newDirCreated) {
   exceptionHandler.printPlainError(
    "Make sure you have permissions set to rwx in your root "
    "directory in "
    "order for us to create your account");
   return false;
  }
  return true;
 }

 vector < string > getUserInfo(bool rawData) {
  fstream *file = fileManager.openFileReadWrite(fileManager.HOME_DIR +
   "/config.yaml");
  if (!file) {
   delete file;
   return {};
  }
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
    {{
     "Please reload the application and try again. We encountered "
     "an "
     "issue reading your configuration file."
    },
     {
      "1. Try deleting your configuration and re-running the "
      "program"
     },
     {
      "2. Make sure you have permissions set correctly to your "
      "file"
     }});
   file->close();
   delete file;
   return {};
  }
  file->close();
  delete file;
  return rows;
 }

 ifstream *checkForLocalConfigFile(const string &fileName) {
  ifstream *fileExists = fileManager.checkExistingFile(fileName);
  if (!fileExists) {
   return nullptr;
  }
  return fileExists;
 }

 ofstream *createConfigFile(const string &fileName) {
  ofstream *newConfig = fileManager.createNewFile(fileName);
  if (!newConfig) {
   delete newConfig;
   bool userInput = exceptionHandler.handleError(
    {{
     "We are having issues initializing a configuration file"
    }},
    "Would you like to create this configuration file manually? "
    "(Y/n)");
   if (!userInput) {
    return nullptr;
   }
   exceptionHandler.printInstructions(
    {{
     "Steps to take: "
    },
     {
      "1. End this program and within your current directory type in "
      "\"touch config.yaml\""
     },
     {
      "2. Run a command to make sure that you have read write and "
      "execution access within the directory \"chmod 777 "
      "config.yaml\""
     },
     {
      "3. You are all set, re-run the application and try again."
     }});
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
    {{
     "it looks like you have started creating an account but never "
     "finished."
    }},
    "Would you like to continue where you left off? (Y/n): ");
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
    } else {
     return 1;
    }
   }
  }
  return 1;
 }

 void createAccount(ofstream *configFile) {
  cout << "Let's create an account" << endl
  << "Welcome to CPP-Notes" << endl;
  string remoteAccount = ioHandler.getInput < string > (
   {{
    "This application is a sister application to Native Notes, and "
    "Electron Notes"
   }},
   "Do you already have an existing account with one of these "
   "platforms? "
   "(Y/n): ",
   "Pleas provide a valid answer");
  if (remoteAccount == "y" || remoteAccount == "Y") {
   cout
   << endl
   << "That is great news! This will be very easy. We will set up "
   "your "
   "account on here with the same credentials as electron notes, "
   "just make sure you provide the same username, password and "
   "email as your online account so we can sync your notes"
   << endl;
   cout
   << endl
   << "If the online account connection fails that is okay. We will "
   "finish creating your account and you can play with your "
   "settings and account info later"
   << endl;
  } else {
   cout << endl
   << "Okay, no problem let's create a new username, email, "
   "password "
   "and a new pin incase you want to lock your notes."
   << endl;
   cout << "We can create an online account later if you want to"
   << endl;
  }
  string newName = createUsername();
  string newEmail = createEmail();
  string newPassword = createPassword();
  int newPin = createPin();
  string mainDir = ioHandler.getInput < string > (
   {{
    "\nWe will be storing all of your notes at the root of your "
    "system "
    "in an accessible manner if you decide to manually update "
    "them."
   }},
   "Main directory: ",
   "Please provide a valid name for this new directory");
  bool newMainDir = nameMainDir("/" + mainDir);
  if (!newMainDir) {
   // Handle try again logic this is necessary
   return;
  }
  bool configInitialized = initializeConfig(
   newName, newEmail, newPassword, newPin, mainDir, configFile);
  if (configInitialized) {
   system("clear");
   cout << "\nWe officially created a new account for you. More "
   "configuration can be done within your settings. Happy notes "
   "taking!!!\n"
   << endl;
   return;
  }
  if (!configInitialized) {
   exceptionHandler.printInstructions(
    {{
     "Steps to take: "
    },
     {
      "1. End this program and within your current directory type in "
      "\"touch config.yaml\""
     },
     {
      "2. Run a command to make sure that you have read write and "
      "execution access within the directory \"chmod 777\""
     },
     {
      "3. You are all set. Restart the application and try again"
     }});
   return;
  }
 }

 void finishCreatingAccount(vector < string > &currentData) {
  size_t length = currentData.size();
  if (length < 1) {
   ofstream *newConfig =
   createConfigFile(fileManager.HOME_DIR + "/" + "config.yaml");
   createAccount(newConfig);
   delete newConfig;
   return;
  }
  if (length == 1) {
   currentData.push_back(createUsername());
   finishCreatingAccount(currentData);
  }
  if (length == 2) {
   currentData.push_back(createEmail());
   finishCreatingAccount(currentData);
  }
  if (length == 3) {
   currentData.push_back(createPassword());
   finishCreatingAccount(currentData);
  }
  if (length == 4) {
   currentData.push_back(to_string(createPin()));
   finishCreatingAccount(currentData);
  }
  if (length == 5) {
   string mainDir = ioHandler.getInput < string > (
    {{
     "\nWe will be storing all of your notes at the root of your "
     "system "
     "in an accessible manner if you decide to manually update "
     "them."
    }},
    "Main directory: ",
    "Please provide a valid name for this new directory");
   bool newMainDirCreated = nameMainDir("/" + mainDir);
   if (!newMainDirCreated) {
    system("clear");
    exceptionHandler.printPlainError(
     "It looks like we need permission to your home directory, or "
     "this "
     "directory name already exsists. Please try again");
    string isUserDir = ioHandler.getInput < string > (
     {{
      "Is " + mainDir + " the directory you want to use? (Y/n): "
     }},
     "", "Please provide a valid answer, Y for yes, n for no");
    if (isUserDir == "Y" || "y") {
     cout << "Okay, we will set this as your main directory" << endl;
     currentData.push_back(mainDir);
     finishCreatingAccount(currentData);
    }
    cout << "Okay, please type in the directory you want to use"
    << endl;
    finishCreatingAccount(currentData);
   }
   currentData.push_back(mainDir);
   finishCreatingAccount(currentData);
  }
  if (length >= 6) {
   ofstream *configFile = fileManager.createNewFile(
    fileManager.HOME_DIR + "/" + "config.yaml");
   bool configInitialized = initializeConfig(
    currentData[1], currentData[2], currentData[3],
    stoi(currentData[4]), currentData[5], configFile);
   if (configInitialized) {
    system("clear");
    cout << "\nWe officially finished creating a new account for you. "
    "More "
    "configuration can be done within your settings. Happy "
    "notes "
    "taking!!!\n"
    << endl;
    return;
   }
   if (!configInitialized) {
    exceptionHandler.printInstructions(
     {{
      "Steps to take: "
     },
      {
       "1. End this program and within your current directory type "
       "in "
       "\"touch config.yaml\""
      },
      {
       "2. Run a command to make sure that you have read write and "
       "execution access within the directory \"chmod 777\""
      },
      {
       "3. You are all set. Restart the application and try "
       "again"
      }});
    return;
   }
   return;
  }
  return;
 }

 void logout() {
  string confirmLogout = ioHandler.getInput < string > (
   {{
    ""
   }}, "Are you sure you want to logout? (Y/n): ",
   "Please provide a valid answer, Y for yes, n for no");
  if (confirmLogout == "Y" || confirmLogout == "y") {
   changeLogin("false");
   return;
  }
  return;
 }

 void deleteAccount() {
  remove("config.yaml");
  return;
 }
};

#endif