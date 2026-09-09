#include <algorithm>
#include <iostream>
#include <limits>
#include <sstream>
#include <vector>

#include "../common/exceptionHandlerInstance.h"
#include "../common/fileManagerInstance.h"
#include "../common/helpers.h"
#include "../common/httpHandlerInstance.h"
#include "../common/ioHandlerInstance.h"
#include "../common/validatorInstance.h"
#include "colors.h"

using namespace std;
using json = nlohmann::json;

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

void loopNestedFolders(const json &folders, const int &folderid) {}

class ConfigManager {
private:
  struct User {
    int userid;
    int pin;
    bool loggedIn;
    bool hasSyncedServer;
    const string &token;
    const string &username;
    const string &email;
    const string &password;
    const string &mainDir;

    User(int userid, int pin, bool loggedIn, bool hasSyncedServer,
         const string &token, const string &username, const string &email,
         const string &password, const string &mainDir)
        : userid(userid), pin(pin), loggedIn(loggedIn),
          hasSyncedServer(hasSyncedServer), token(token), username(username),
          email(email), mainDir(mainDir) {}
  };

  inline static globalUser =
      User(1, 1234, false, false, "", "", "", "", "/cpp-notes");

  void confirmPass(const string &password) {
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

    fstream *file =
        fileManager.openFileReadWrite(fileManager.HOME_DIR + "config.yaml");

    if (!file) {
      // Do not catch this. Allow dev to fix
      throw runtime_error("Dev: opening config.yaml file from inside "
                          "changeLogin is failing. Check for proper routing");
    }

    rows[0] = "logged_in: " + state;

    try {
      for (const string &row : rows) {
        *file << row << "\n";
      }
    } catch (const filesystem_error &err) {
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
    if (!validator.checkValidString(3, 20,
                                    {'<', '>', ',', '{', '}', '[', ']', '!',
                                     '@', '#', '$', '%', '^', '&', '*', '(',
                                     ')', '+', '='},
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
    if (Helpers::inputStringIsYes(confirm)) {
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

    if (Helpers::inputStringIsYes(confirm)) {
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
    if (Helpers::inputStringIsYes(confirm)) {
      return pin;
    } else {
      system("clear");
      cout << BLUE + "Okay, try again" + ENDCOLOR << endl;
      return createPin();
    }
    return pin;
  };

  string createCustomDirName() {
    string dirName =
        ioHandler.getInput<string>({""},
                                   "What would you like your directory to be "
                                   "called? Do not use \"/\" characters: ",
                                   "Please provide a valid response and "
                                   "remember to not use \"/\" characters");

    if (dirName.size() < 1) {
      return "cpp-notes";
    }

    return Helpers::mainDirStringCleanup(dirName);
  }

  // USAGE: Update / create the most up to date User struct available to the
  // main configuration file.

  // NOTE: Call after updating global user to keep config file in sync. Global
  // search "globalUser = User("; and call this method after each.
  void writeToConfigFile() {
    ofstream *configFile = openNewConfig();

    if (!configFile) {
      return;
    }

    *configFile << "userid: " << globalUser.userid << "\n";
    *configFile << "token: " << globalUser.token << "\n";
    *configFile << "loggedIn: " << boolalpha << globalUser.loggedIn << "\n";
    *configFile << "hasSyncedServer: " << boolalpha
                << globalUser.hasSyncedServer << "\n";
    *configFile << "username: " << globalUser.username << "\n";
    *configFile << "email: " << globalUser.email << "\n";
    *configFile << "password: " << globalUser.password << "\n";
    *configFile << "pin: " << globalUser.pin << "\n";
    *configFile << "maindir: " << "/" + globalUser.mainDir << "\n";

    if (configFile->fail()) {
      // NOTE: Catch this method if program should continue even without
      // successfull write
      throw runtime_error("Could not write to your configuration file. Storage "
                          "may be low or some other io operation failed");
    }

    configFile->close();
    delete configFile;
  }

  string getNewUsername(const string &currentUsername) {
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

  string getNewPass(const string &currentPass) {
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

  int getNewPin(const int &currentPin) {
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

  bool updateConfig(const vector<string> &userInfo) {
    fstream *config =
        fileManager.openFileReadWrite(fileManager.HOME_DIR + "/config.yaml");
    if (!config) {
      delete config;
      return false;
    }
    *config << "userid: " << "" << "\n";
    *config << "token: " << "" << "\n";
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
  // NOTE: Remember to update config if it already exists after opening. This
  // method opens an existing config if it already has been created, truncates
  // and erases all text that is already present returning a pointer to an empty
  // blank config file
  ofstream *openNewConfig() {
    ofstream *newConfig =
        createConfigFile(fileManager.HOME_DIR + "/config.yaml");

    // Failed to load a new config file in root. Kill app and prompt user
    if (!newConfig) {
      delete newConfig;
      exceptionHandler.printPlainError(
          "Please make sure the app has sufficient permissions to create files "
          "in root. Reload the application and try again.");
      return nullptr;
    }

    return newConfig;
  }

  bool changeUsername(vector<string> &userInfo) {
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

  bool changePass(vector<string> &userInfo) {
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

  bool changePin(vector<string> &userInfo) {
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

  bool changeDir(vector<string> &userInfo) {
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
    if (Helpers::inputStringIsYes(confirmation)) {
      const string newDirName = ioHandler.getInput<string>(
          {{""}}, "New directory name: ", "Please provide a valid response");
      // Validate new directory name
      const string confirmNewDirName = ioHandler.getInput<string>(
          {{""}},
          "Confirm this is your new directory name: " + YELLOW + newDirName +
              ENDCOLOR,
          "Please provide a valid response");

      if (Helpers::inputStringIsYes(confirmNewDirName)) {
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

  void createMainDir(const string &mainDirName) {
    bool newDirCreated = fileManager.createNewDir("/" + mainDir);

    // NOTE: Program must hault and user manages problem. Cannot move on without
    // main dir
    if (!newDirCreated) {
      throw runtime_error(
          "Please check permissions or the way you are attempting to write the "
          "name of your main directory");
    }
  }

  vector<string> getUserInfo(bool rawData) {
    fstream *file =
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

  // TODO: Make a better check for existing account method. Delete old config if
  // not finished. Make sure all rows exist by name and value. Make sure config
  // is not tampered with or messed up
  bool checkForExistingAccount() {
    bool confirmed = false;
    vector<string> rows = getUserInfo(false);

    if (rows.size() < 9) {
      return false;
    }

    return true;
  }

  void manageUser(const string &token, const json &user) {
    // NOTE: Server user data looks like:
    // { user: userid: int, username: string, email: string, createdat: int
    // };

    if (!exceptionHandler.containsAll({"userid", "username", "email"}, user)) {
      exceptionHandler.printPlainError(
          "There was a problem grabbing your information from the server. Try "
          "logging in again");
      loginServer();
      return;
    }

    // NOTE: We know it is safe to grab userid, username, email as we already
    // checked for these values in grabServerData();
    int userid = user["userid"].get<int>();
    const string &username = user["username"].get<string>();
    const string &email = user["email"].get<string>();
    int newPin = createPin();

    string mainDirName = "cpp-notes";
    string makeCustomDir = ioHandler.getInput<string>(
        {""},
        "Would you like to create a custom directory name for your notes to be "
        "stored in or go with the defualt \"cpp-notes\" directory? (y/N)",
        "Please provide a valid response. \"y\" for yes and \"n\" for no");

    if (Helpers::inputStringIsYes(makeCustomDir)) {
      mainDirName = createCustomDirName();
    }

    createMainDir(mainDirName);

    globalUser = User(userid, newPin, true, true, token, username, email,
                      "/" + mainDirName);
    writeToConfigFile();
  }

  inline static vector<int> parentFolderIdsToIgnore = {};

  bool folderChecksPass(const string &title, const int &id) {
    if (find(parentFolderIdsToIgnore.begin(), parentFolderIdsToIgnore.end(),
             id) != parentFolderIdsToIgnore.end()) {
      return false;
    }

    // FIX: Do not leave this as is! Need to instead navigate into dir and
    // continue on as though that folder was successfully created and navigated
    // to
    if (fileManager.checkDirExists(title)) {
      return false;
    }

    return true;
  }

  bool didCreateDirAndNavigate(const string &title) {
    bool didCreateNewDir = fileManager.createNewDirCustom("/" + title);

    if (!didCreateNewDir) {
      return false;
    }

    bool didNav = fileManager.navigateDir(title);

    if (!didNav) {
      throw runtime_error(
          "Could not navigate through your file system. Check for "
          "malformed folder names or contact the developer");
    }

    return true;
  }

  void updateLoop(const json &folders, const int &id) {
    parentFolderIdsToIgnore.push_back(id);
    loopNestedFolders(folders, id);
  }

  void loopNestedFolders(const json &folders, const int &folderid) {

    // WARNING: We need to find a better way to know that we have traversed to
    // the top level again more securely
    if (fileManager.isHome()) {
      return;
    }

    for (const json &folder : folders) {
      if (!Helpers::containsAll({"title", "folderid", "parentFolderId"},
                                folder)) {
        // TODO: Maybe skip this folder?? Add it to some error sync log
        continue;
      }

      int id = folder.at("folderid").get<int>();
      string title = folder.at("title").get<string>();
      bool isTopLevelFolder = folder.at("parentFolderId").is_null();

      if (isTopLevelFolder) {
        continue;
      }

      if (!folderChecksPass(title, id)) {
        continue;
      }

      int parentFolderId = folder.at("parentFolderId").get<int>();

      if (parentFolderId != folderid) {
        continue;
      }

      if (!didCreateDirAndNavigate(title)) {
        continue;
      }

      updateLoop(folders, id);
    }

    fileManager.navBack();
  }

  // WARNING: Be careful where the user is currently at in the filesystem
  // directory before calling this method
  void manageUserData(const json &folders, const json &notes) {
    // NOTE: Depth first search recursion pattern
    for (const json &folder : folders) {
      if (!Helpers::containsAll({"title", "folderid", "parentFolderId"},
                                folder)) {
        // TODO: Maybe skip this folder?? Add it to some error sync log
        continue;
      }

      int id = folder.at("folderid").get<int>();
      string title = folder.at("title").get<string>();
      bool isTopLevelFolder = folder.at("parentFolderId").is_null();

      if (!isTopLevelFolder) {
        continue;
      }

      // NOTE: Do not create the directory. It already has been looped
      // over or created
      if (!folderChecksPass(title, id)) {
        continue;
      }

      // NOTE: Build folder in main dir
      if (!didCreateDirAndNavigate(title)) {
        continue;
      }

      updateLoop(folders, id);
    }

    if (folders.size() > 0) {
      fileManager.navBack();
    }
  }

  void grabServerData(const string &token, HttpHandler &httpHandler) {
    HttpHandler::ResponseObject res = httpHandler.getUserData(token);

    if (res.messageFromServer.size() > 0) {
      exceptionHandler.printPlainError(res.resBodyString);
    }

    if (res.didFail) {
      exceptionHandler.printPlainError(
          "We failed to synchronize your existing data from the server. Try to "
          "log in again.");
      loginServer();
      return;
    }

    if (res.bodyIsString) {
      exceptionHandler.printStringResBody(res.httpCode, res.resBodyString);
    }

    const json &body = res.resBodyJson;
    const json &data = body["data"].get<json>();

    if (!Helpers::containsAll({"user", "folders", "notes"}, data)) {
      exceptionHandler.printPlainError(
          "There was a problem fetching your data from the server. Please try "
          "logging in again");
      loginServer();
      return;
    }

    // TODO: Validate server response with .is_array && .is_object
    const json &user = data.at("user");
    const json &folders = data.at("folders");
    const json &notes = data.at("notes");

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

    // TODO: Make sure that email and password are validated
    string email = ioHandler.getInput<string>(
        {"What is your email associated with your account?"},
        "Email: ", "Please provide a valid email");
    string password = ioHandler.getInput<string>(
        {"What is your password associated with your account?"},
        "Password: ", "Plese input a valid password");

    // NOTE: Create httpHanlder instance and reuse until api calls are finished
    // in the chain to prevent copying and lost memory
    HttpHandler httpHandler;

    HttpHandler::ResponseObject res =
        httpHandler.login(username, email, password);

    if (res.messageFromServer.size() > 0) {
      exceptionHandler.printPlainError(YELLOW + res.messageFromServer +
                                       ENDCOLOR);
      return;
    }

    if (res.didFail) {
      string reLogin = ioHandler.getInput<string>(
          {"Login failed"},
          "Would you like to attempt login one more time? (y,N): ",
          "Please input a valid answer \"y\" for yes and \"n\" for no");

      if (Helpers::inputStringIsYes(reLogin)) {
        loginServer();
        return;
      }

      // NOTE: Login failed, so send the user back to the begining and prompt
      // for new account creation
      ofstream *newConfig = openNewConfig();

      if (!newConfig) {
        // NOTE: Config file creation failed. Kill program. Prompt for fix
        // already handled in openNewConfig()
        return;
      }

      createAccount(newConfig);
      newConfig->close();
      delete newConfig;
    }

    if (res.bodyIsString) {
      exceptionHandler.printStringResBody(res.httpCode, res.resBodyString);
      return;
    }

    const string &token = res.resBodyJson["data"].get<string>();

    grabServerData(token, httpHandler);
  }

  void createAccount() {
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
        "Please answer with \"y\" for yes or \"N\" for no");

    if (Helpers::inputStringIsYes(hasSisterAccount)) {
      // NOTE: Config is deleted here. Config file can be created later instead
      // of passing it along with continuous API calls after loginServer();

      // TODO: Finish off with manage user notes and folders
      loginServer();
      return;
    }

    int id = 0;
    bool hasSyncedServer = false;
    string newName = createUsername();
    string newEmail = createEmail();
    string newPassword = createPassword();
    int newPin = createPin();

    string mainDirName = createCustomDirName();

    // TODO: Decide if catching is worth it here and with writeToConfigFile
    // below
    createMainDir(mainDirName);

    globalUser = User(id, newPin, true, false, "", newName, newEmail,
                      newPassword, "/" + mainDirName);

    // NOTE: Adding try catch block because it is users first time attempting
    // account creation so the code can handle a retry
    writeToConfigFile();

    system("clear");
    cout << "We officially created a new account for you. More "
            "configuration can be done within your settings. Happy note "
            "taking!!!"
         << endl
         << endl;
  }

  bool logout() {
    string confirmLogout = ioHandler.getInput<string>(
        {{""}}, YELLOW + "Are you sure you want to logout? (Y/n): " + ENDCOLOR,
        "Please provide a valid answer, Y for yes, n for no");

    if (Helpers::inputStringIsYes(confirmLogout)) {
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
    } catch (const filesystem_error &err) {
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