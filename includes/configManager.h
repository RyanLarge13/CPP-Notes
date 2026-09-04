#include <algorithm>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

#include "../common/exceptionHandlerInstance.h"
#include "../common/fileManagerInstance.h"
#include "../common/httpHandlerInstance.h"
#include "../common/ioHandlerInstance.h"
#include "../common/validatorInstance.h"
#include "colors.h"

using namespace std;
using json = nlohmann::json;

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

class ConfigManager {
 private:
  void confirmPass(const string& password) {
    string confirmPassword = ioHandler.getInput<string>(
        {{"Confirm your password"}},
        "Confirm Password: ", "Please input valid characters");
    if (!validator.checkValidString(8, 20, {';'}, confirmPassword)) {
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
                                       [](int ch) { return !isspace(ch); }));
    value.erase(find_if(value.rbegin(), value.rend(),
                        [](int ch) { return !isspace(ch); })
                    .base(),
                value.end());
    return value;
  }

  bool changeLogin(string state) {
    if (state != "true" || state != "false") {
      // Do not catch this error. Should stop application and trigger developer
      // fix
      throw runtime_error(
          "Dev: pass a valid true or false string to changeLogin()");
    }

    vector<string> rows = getUserInfo(true);

    if (rows.empty()) {
      // Do not catch this. Allow dev to fix
      throw runtime_error(
          "Dev: getUserInfo is returning no data. Check method implementation "
          "when calling getUserInfo from inside changeLogin()");
    }

    fstream* file =
        fileManager.openFileReadWrite(fileManager.HOME_DIR + "config.yaml");

    if (!file) {
      // Do not catch this. Allow dev to fix
      throw runtime_error(
          "Dev: opening config.yaml file from inside "
          "changeLogin is failing. Check for proper routing");
    }

    rows[0] = "logged_in: " + state;

    try {
      for (const string& row : rows) {
        *file << row << "\n";
      }
    } catch (const filesystem_error& err) {
      exceptionHandler.printPlainError(
          "There was a problem writing to your configuration file. Please "
          "check to make sure you have the proper access rights to config.yaml "
          "in your root dir at: " +
          fileManager.HOME_DIR);
      return false;
    }

    file->close();
    delete file;
    return true;
  }

  bool login() {
    int pin = ioHandler.getInput<int>(
        {{YELLOW + "You are logged out" + ENDCOLOR}},
        "Login with your pin: ", "Your pin will be a 4 digit number");

    if (!validator.checkValPin(pin, 1111, 9999, 4)) {
      exceptionHandler.printPlainError("Please respond with a valid pin");
      exceptionHandler.printInstructions(
          {{"- Must be 4 digits", "- No less than 1111",
            "- No greater than 9999", "Please try again"}});

      return login();
    }

    vector<string> rows = getUserInfo(false);

    // Check if the pin stored in users config matches
    // Later implement robust security like encryotion and keys
    if (stoi(rows[4]) == pin) {
      system("clear");
      bool loginSuccess = changeLogin("true");

      if (loginSuccess) {
        return true;
      }

      // In the future instead of erroring out and forcing a user into infinite
      // login attempts because of fauled changeLogin() calls, send an error to
      // the server for bug handling. For now just return true
      return true;
    }

    exceptionHandler.printPlainError("Incorrect pin. Please try again");
    return login();
  }

  string createUsername() {
    string username = ioHandler.getInput<string>(
        {{""}}, "Username: ", "Your username must be valid characters");
    if (!validator.checkValidString(
            3, 20,
            {'<', '>', ',', '{', '}', '[', ']', '!', '@', '#', '$', '%', '^',
             '&', '*', '(', ')', '+', '='},
            username)) {
      exceptionHandler.printPlainError("Please insert a valid username");
      exceptionHandler.printInstructions(
          {{"- Can ONLY contain:", "  - letters", "  - numbers",
            "  - underscores", "  - and dashes",
            "- Must be at least 3 characters long",
            "- Cannot be longer than 20 characters\n"}});
      return createUsername();
    }
    string confirm = ioHandler.getInput<string>(
        {{"Confirm " + RED + username + ENDCOLOR +
          " is the username you want"}},
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
    string email = ioHandler.getInput<string>(
        {{""}}, "New Email: ", "Please provide a valid email address");
    if (!validator.checkValidString(
            6, 50,
            {{'<', '>', ',', '{', '}', '[', ']', '!', '#', '$', '%', '^', '&',
              '*', '(', ')', '+', '='}},
            email)) {
      exceptionHandler.printPlainError("Please provide a valid email");
      exceptionHandler.printInstructions(
          {{"- Must be at least 6 characters",
            "- Cannot be longer than 50 characters",
            "- Cannot contain these special characters:",
            "'<', '>', ',', '.', '{', '}', '[', ']', '! , '#', '$', '%', '^', "
            "'&', '*', '(', ')', '+','='"}});
      return createEmail();
    }
    string confirm = ioHandler.getInput<string>(
        {{"Confirm " + RED + email + ENDCOLOR + " is the email you want"}},
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
    string password = ioHandler.getInput<string>(
        {{"\nWe strongly suggest a secure and strong password containing:"},
         {"- At least 8 characters *"},
         {"- upper and lowercase characters"},
         {"- numbers"},
         {"- and special characters\n"}},
        "New Password: ", "Please create a valid password");
    if (!validator.checkValidString(8, 50, {';'}, password)) {
      system("clear");
      exceptionHandler.printPlainError(
          "Please input a valid password, quit the program with Ctrl + c");
      return createPassword();
    }
    confirmPass(password);
    return password;
  };

  int createPin() {
    int pin = ioHandler.getInput<int>(
        {{""}}, "New 4-digit pin: ", "Your new pin must be a 4-digit number");
    if (!validator.checkValPin(pin, 1111, 9999, 4)) {
      exceptionHandler.printPlainError("Please input a valid pin");
      exceptionHandler.printInstructions(
          {{"- Must be 4 digits", "- Cannot be less then 1111",
            "- Cannot be greater than 9999",
            "Please try again. If you want to exit press Ctrl + c to quit "
            "at anytime"}});
      return createPin();
    }
    string confirm = ioHandler.getInput<string>(
        {{"Confirm " + RED + to_string(pin) + ENDCOLOR +
          " is what you want your new pin to be"}},
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

  bool initializeConfig(const string& username, const string& email,
                        string password, int pin, const string& mainDir,
                        ofstream* configFile) {
    if (!configFile) {
      cout << "No config file defined canceling initialize config" << endl;
    }
    if (username.size() < 1 || email.size() < 1 || password.size() < 1 ||
        pin < 1000) {
      exceptionHandler.printPlainError(
          "You must complete the registration "
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

  string getNewUsername(const string& currentUsername) {
    const string newUsername = ioHandler.getInput<string>(
        {{"Type your current username again to cancel and return to the "
          "main "
          "menu"}},
        "\nNew username: ", "Please provide a valid new username");
    if (!validator.checkValidString(
            3, 20,
            {{'<', '>', ',', '{', '}', '[', ']', '!', '@', '#', '$', '%', '^',
              '&', '*', '(', ')', '+', '='}},
            newUsername)) {
      exceptionHandler.printPlainError("Please insert a valid username");
      exceptionHandler.printInstructions(
          {{"- Can ONLY contain:", "  - letters", "  - numbers",
            "  - underscores", "  - and dashes",
            "- Must be at least 3 characters long",
            "- Cannot be longer than 20 characters\n"}});
      return getNewUsername(currentUsername);
    }
    if (newUsername == currentUsername) {
      system("clear");
      cout << "Canceling" << endl;
      return "####";
    }
    const string confirmNewName = ioHandler.getInput<string>(
        {{""}},
        "Confirm your new username: ", "Please provide a valid confirmation");
    if (confirmNewName != newUsername) {
      cout << "Please try again" << endl;
      return getNewUsername(currentUsername);
    }
    return newUsername;
  }

  string getNewPass(const string& currentPass) {
    const string newPass = ioHandler.getInput<string>(
        {{""}}, "New password: ", "Please provide a valid response");
    if (newPass == currentPass) {
      return "####";
    }
    // Check validation
    const string confirmPass = ioHandler.getInput<string>(
        {{""}}, "Confirm new password: ", "Please provide a valid response");
    if (confirmPass != newPass) {
      exceptionHandler.printPlainError(
          "Please type the exact same password. Try again");
      return getNewPass(currentPass);
    }
    return newPass;
  }

  int getNewPin(const int& currentPin) {
    const int newPin =
        ioHandler.getInput<int>({{"Enter your original pin value to cancel and "
                                  "return to the main menu"}},
                                "New pin: ", "Please provide a valid response");
    // Validate new pin
    if (newPin == currentPin) {
      return -999;
    }
    const int confirmNewPin = ioHandler.getInput<int>(
        {{""}}, "Confirm your new pin: ", "Please provide a valid response");
    if (confirmNewPin != newPin) {
      exceptionHandler.printPlainError(
          "You must confirm your new pin. Please try again");
      return getNewPin(currentPin);
    }
    return newPin;
  }

  bool updateConfig(const vector<string>& userInfo) {
    fstream* config =
        fileManager.openFileReadWrite(fileManager.HOME_DIR + "/config.yaml");
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
  struct User {
    string userid;
    string username;
    string email;
    string token;

    User(const string& userid, const string& username, const string& email,
         const string& token)
        : userid(userid), username(username), email(email), token(token) {}
  };

  inline static User globalUser = User("0", "", "", "");

  bool changeUsername(vector<string>& userInfo) {
    cout << "Okay, let's change your username. To exit, simply type your "
            "current username when asked to give a new one"
         << endl;
    // If online account connected. Also change remote username.
    const string currentUsername = userInfo[1];
    const string confirmName = ioHandler.getInput<string>(
        {{""}}, "Confirm current username: ", "Please input a valid username");
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
      userInfo[1] = currentUsername;
      exceptionHandler.printPlainError(
          "We could not update your local configuration.. We are terribly "
          "sorry. Try changing your username again.");
    }
    return didUpdate;
  }

  bool changePass(vector<string>& userInfo) {
    const string currentPass = userInfo[3];
    cout << "Okay, let's change your password. To cancel and return to main "
            "menu type in your current password again when asked for a new "
            "one"
         << endl;
    const string confirmPass = ioHandler.getInput<string>(
        {{""}}, "Current Password: ", "Please provide a valid response");
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

  bool changePin(vector<string>& userInfo) {
    const int currentPin = stoi(userInfo[4]);
    cout << "Okay, sounds good. let's change your pin for logging in and "
            "opening locked notes"
         << endl;
    const int confirmPin = ioHandler.getInput<int>(
        {{""}}, "Confirm your current pin: ", "Please provide a valid pin");
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

  bool changeDir(vector<string>& userInfo) {
    const string currentMainDir = userInfo[5];
    cout << "Okay, let's change the directory name that you store your "
            "folders "
            "in"
         << endl;
    const string confirmation =
        ioHandler.getInput<string>({{""}},
                                   "Are you sure you would like to change the "
                                   "name of your main directory? (Y/n): ",
                                   "Pleas provide a valid response");
    if (confirmation == "Y" || confirmation == "y") {
      const string newDirName = ioHandler.getInput<string>(
          {{""}}, "New directory name: ", "Please provide a valid response");
      // Validate new directory name
      const string confirmNewDirName = ioHandler.getInput<string>(
          {{""}},
          "Confirm this is your new directory name: " + YELLOW + newDirName +
              ENDCOLOR,
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

  bool nameMainDir(const string& dirname) {
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

  vector<string> getUserInfo(bool rawData) {
    fstream* file =
        fileManager.openFileReadWrite(fileManager.HOME_DIR + "/config.yaml");
    if (!file) {
      delete file;
      return {};
    }
    string line;
    string value;
    vector<string> rows;
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
          {{"Please reload the application and try again. We encountered "
            "an "
            "issue reading your configuration file."},
           {"1. Try deleting your configuration and re-running the "
            "program"},
           {"2. Make sure you have permissions set correctly to your "
            "file"}});
      file->close();
      delete file;
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
          {{"We are having issues initializing a configuration file"}},
          "Would you like to create this configuration file manually? "
          "(Y/n)");
      if (!userInput) {
        return nullptr;
      }
      exceptionHandler.printInstructions(
          {{"Steps to take: "},
           {"1. End this program and within your current directory type in "
            "\"touch config.yaml\""},
           {"2. Run a command to make sure that you have read write and "
            "execution access within the directory \"chmod 777 "
            "config.yaml\""},
           {"3. You are all set, re-run the application and try again."}});
      return nullptr;
    }
    return newConfig;
  }

  // Checking if account is fully updated and complete and exists
  // returning 1 means incomplete account. 0 means complete and logged in. 3
  // means exit app
  int checkForExistingAccount() {
    bool confirmed = false;
    vector<string> rows = getUserInfo(false);

    if (rows.size() == 0) {
      return 1;
    }

    if (rows.size() < 6) {
      bool userPicksUpWhereLeftOff = exceptionHandler.handleError(
          {{"it looks like you have started creating an account but never "
            "finished."}},
          "Would you like to continue where you left off? (Y/n): ");

      if (userPicksUpWhereLeftOff) {
        return 1;
      } else {
        return 3;
      }
    }

    if (rows.size() >= 6) {
      string loggedInLine = rows[0];
      if (loggedInLine == "true") {
        return 0;
      }
      if (loggedInLine == "false") {
        system("clear");
        login();

        // Returning 0 to caller from main.cpp prints menu. Only a successful
        // login returns
        return 0;
      }
    }

    return 1;
  }

  void manageUser(const string& token, const json& user) {
    // user = userid, username, email, createdat
    // also save token, but make sure it is serialized

    const string& userid = user["userid"].get<string>();
    const string& username = user["username"].get<string>();
    const string& email = user["email"].get<string>();

    globalUser = User(userid, username, email, token);
  }

  void manageUserData(const json& folders, const json& notes) {}

  void grabServerData(const string& token, HttpHandler& httpHandler) {
    HttpHandler::HttpResponse res = httpHandler.getUserData(token);

    bool didFail =
        httpHandler.handleCurlOrHttpCodeInformation(res.curlCode, res.httpCode);

    if (didFail) {
      const json& body = get<json>(res.body);
      const string& messageFromServer = body["data"]["message"].get<string>();

      if (messageFromServer.size() > 0) {
        exceptionHandler.printPlainError(
            "You have a message from the server involving your last request: " +
            YELLOW + messageFromServer + ENDCOLOR);
      }

      return;
    }

    if (holds_alternative<string>(res.body)) {
      const json& body = get<json>(res.body);
      exceptionHandler.printStringResBody(res.httpCode, body);
      return;
    }

    // Lots of json for days here. Don't forget to use references with & and
    // .get<...>(); for extracting values correctly
    const json& body = get<json>(res.body);

    const json& user = body["data"]["user"].get<json>();
    const json& folders = body["data"]["folders"].get<json>();
    const json& notes = body["data"]["notes"].get<json>();

    manageUser(token, user);
    manageUserData(folders, notes);
  }

  void loginServer() {
    string username = ioHandler.getInput<string>(
        {"What is the username for your account?"},
        "Username: ", "Please provide a valid username");

    bool isValidUsername = validator.checkValidString(2, 20, {}, username);

    if (!isValidUsername) {
      exceptionHandler.printPlainError("Please provide a valid username");
      loginServer();
      return;
    }

    // TODO -------------------------------
    // 1. Validate email and password
    string email = ioHandler.getInput<string>(
        {"What is your email associated with your account?"},
        "Email: ", "Please provide a valid email");

    string password = ioHandler.getInput<string>(
        {"What is your password associated with your account?"},
        "Password: ", "Plese input a valid password");

    HttpHandler httpHandler;
    HttpHandler::HttpResponse res =
        httpHandler.login(username, email, password);

    bool didFail =
        httpHandler.handleCurlOrHttpCodeInformation(res.curlCode, res.httpCode);

    if (didFail) {
      const json& body = get<json>(res.body);
      string messageFromServer = body["data"]["message"].get<string>();

      if (messageFromServer.size() > 0) {
        exceptionHandler.printPlainError(
            "You have a message from the server involving your last request: " +
            YELLOW + messageFromServer + ENDCOLOR);
      }

      return;
    }

    if (holds_alternative<string>(res.body)) {
      const json& body = get<string>(res.body);
      exceptionHandler.printStringResBody(res.httpCode, body);
      return;
    }

    const json& body = get<json>(res.body);
    const string& token = body["data"]["token"].get<string>();

    grabServerData(token, httpHandler);
  }

  // Create or login to existing account for Electron/ Native Notes
  // ---------------------------
  void createAccount(ofstream* configFile) {
    cout << "Let's create an account" << endl
         << endl
         << YELLOW + "Welcome to CPP-Notes" + ENDCOLOR << endl
         << endl;

    string hasSisterAccount = ioHandler.getInput<string>(
        {"We have sister applications called" + YELLOW + " Electron Notes" +
             ENDCOLOR + " for desktop and " + YELLOW + "Native Notes" +
             ENDCOLOR + " for Android",
         "You can login here with those credentials if you already use those "
         "applications and keep all of your notes synced here as well."},
        "Do you have an account with one of these sister applications? (y,N): ",
        "Please answer with y for yes or N for no");

    if (hasSisterAccount == "y" || hasSisterAccount == "Y") {
      loginServer();
      return;
    }

    string newName = createUsername();
    string newEmail = createEmail();
    string newPassword = createPassword();
    int newPin = createPin();
    string mainDir = ioHandler.getInput<string>(
        {{"\nWe will be storing all of your notes at the root of your "
          "system "
          "in an accessible manner if you decide to manually update "
          "them."}},
        "Main directory: ",
        "Please provide a valid name for this new directory");
    bool newMainDir = nameMainDir("/" + mainDir);
    if (!newMainDir) {
      // Handle try again logic this is necessary
      return;
    }
    bool configInitialized = initializeConfig(newName, newEmail, newPassword,
                                              newPin, mainDir, configFile);
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
          {{"Steps to take: "},
           {"1. End this program and within your current directory type in "
            "\"touch config.yaml\""},
           {"2. Run a command to make sure that you have read write and "
            "execution access within the directory \"chmod 777\""},
           {"3. You are all set. Restart the application and try again"}});
      return;
    }
  }

  void finishCreatingAccount(vector<string>& currentData) {
    size_t length = currentData.size();
    if (length < 1) {
      ofstream* newConfig =
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
      string mainDir = ioHandler.getInput<string>(
          {{"\nWe will be storing all of your notes at the root of your "
            "system "
            "in an accessible manner if you decide to manually update "
            "them. Choose a folder name to store your notes in."}},
          "Folder name: ",
          "Please provide a valid name for this new directory");
      bool newMainDirCreated = nameMainDir("/" + mainDir);
      if (!newMainDirCreated) {
        system("clear");
        exceptionHandler.printPlainError(
            "It looks like we need permission to your home directory, or "
            "this "
            "directory name already exsists. Please try again");
        string isUserDir = ioHandler.getInput<string>(
            {{"Is " + mainDir + " the directory you want to use? (Y/n): "}}, "",
            "Please provide a valid answer, Y for yes, n for no");
        if (isUserDir == "Y" || "y") {
          cout << "Okay, we will set this as your main directory" << endl;
          currentData.push_back(mainDir);
          finishCreatingAccount(currentData);
        }
        cout << "Okay, please type in the directory you want to use" << endl;
        finishCreatingAccount(currentData);
      }
      currentData.push_back(mainDir);
      finishCreatingAccount(currentData);
    }
    if (length >= 6) {
      ofstream* configFile =
          fileManager.createNewFile(fileManager.HOME_DIR + "/" + "config.yaml");
      bool configInitialized =
          initializeConfig(currentData[1], currentData[2], currentData[3],
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
            {{"Steps to take: "},
             {"1. End this program and within your current directory type "
              "in "
              "\"touch config.yaml\""},
             {"2. Run a command to make sure that you have read write and "
              "execution access within the directory \"chmod 777\""},
             {"3. You are all set. Restart the application and try "
              "again"}});
        return;
      }
      return;
    }
    return;
  }

  bool logout() {
    string confirmLogout = ioHandler.getInput<string>(
        {{""}}, YELLOW + "Are you sure you want to logout? (Y/n): " + ENDCOLOR,
        "Please provide a valid answer, Y for yes, n for no");

    if (confirmLogout == "Y" || confirmLogout == "y") {
      changeLogin("false");
      return true;
    }

    return false;
  }

  bool deleteAccount() {
    bool confirmed = exceptionHandler.handleError(
        {YELLOW +
         "If you delete your account all folders and notes will be lost if you "
         "have not backed them up." +
         ENDCOLOR},
        "Are you sure you want to continue with this operation? (Y/n): ");

    if (!confirmed) {
      return false;
    }

    try {
      bool fileDeleted =
          fileManager.deleteFile(fileManager.HOME_DIR + "/config.yaml");

      if (!fileDeleted) {
        system("clear");
        exceptionHandler.printPlainError(
            YELLOW +
            "Your account file could not be deleted. Find config.yaml and "
            "remove it" +
            ENDCOLOR);
        return false;
      }

      return true;
    } catch (const filesystem_error& err) {
      system("clear");
      exceptionHandler.printPlainError(
          YELLOW +
          "Your account file could not be deleted. Find config.yaml and "
          "remove it" +
          ENDCOLOR);
      return false;
    }
  }
};

#endif