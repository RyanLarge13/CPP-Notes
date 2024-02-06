#include <iostream>
#include <vector>
#include <limits>
#include <filesystem>
#include <fstream>
#include <limits>
#include "./includes/filemanager.h"
#include "./includes/configManager.h"
#include "./includes/httpHandler.h"

using namespace std;
namespace fs = std::filesystem;

FileManager fileManager;
ConfigManager configManager;

const vector < string > options = {
  "1. New Note",
  "2. New Folder",
  "3. Settings",
  "4. Logout",
  "5. Delete Account"
};

void printMenu() {
  bool getOption = true;
  vector < string > userInfo = configManager.getUserInfo();
  cout << "Welcome " << userInfo[0] << endl;
  cout << "**************" << endl;
  for (string option: options) {
    cout << option << endl;
  }
  int selection;
  int tries = 0;
  while (getOption) {
    cin >> selection;
    if (cin.fail()) {
      cin.clear();
      cin.ignore(numeric_limits < streamsize > ::max());
    if (tries == 3) {
      return;
    }
    cout << "Please select a valid option from the menu. If you are trying to exit the program type 10" << endl;
    tries++;
  }
}
}

void checkForAccount() {
bool configExist = configManager.checkForLocalConfigFile();
if (!configExist) {
configManager.createConfigFile("config.yaml");
configManager.createAccount();
}
if (configExist) {
bool accountEstablished = configManager.checkForExistingAccount();
if (accountEstablished) {
printMenu();
}
else {
configManager.createAccount();
}
}
}

int main(int argc, char* argv[]) {
system("clear");
checkForAccount();
return 0;
}