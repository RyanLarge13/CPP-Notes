#include <iostream>
#include <vector>
#include <ncurses.h>
#include <limits>
#include <filesystem>
#include <fstream>
#include <limits>
#include "../includes/fileManager.h"
#include "../includes/configManager.h"
#include "../includes/ioHandler.h"
#include "../includes/httpHandler.h"
#include "../includes/stateManager.h"
#include "../includes/exceptionHandler.h"

using namespace std;
namespace fs = filesystem;

FileManager fileManager;
ConfigManager configManager;
StateManager stateManager;
ExceptionHandler exceptionHandler;
IoHandler ioHandler;

void printMenu();
void printSettings();
vector < string > userInfo;

const vector < string > options = {
 "1. New Note",
 "2. New Folder",
 "3. Settings",
 "4. Logout",
 "5. Quit (stay logged in)",
 "6. Delete Account"
};

const vector < string > settings = {
 "1. Update profile",
 "2. Create option",
 "3. Main menu"
};

void createNewFile(const string& fileName) {
 ofstream* newFile = fileManager.createNewFile(fileName);
 if (!newFile) {
  exceptionHandler.printPlainError("We could not create a new file on your device. Please make sure you give rwx permissions for this directory");
  delete newFile;
 }
 initscr();
 raw();
 keypad(stdscr, TRUE);
 noecho();
 int ch;
 int x;
 int y;
 string text;
 //printw(fileName + "\n\n");
 move(1, 0);
 while ((ch = getch()) != 27) {
  switch (ch) {
   case KEY_BACKSPACE:
   case 127: {
    if (!text.empty()) {
     text.pop_back();
     getyx(stdscr, y, x);
     if (x > 0) x--;
     if (y > 1) {
      y--;
      x = getmaxx(stdscr) - 1;
     }
     mvdelch(y, x);
     refresh();
    }
   }
   break;
   case '\n':
   text += "\n";
   getyx(stdscr, y, x);
   move(y + 1, 0);
   refresh();
   break;
   default:
   text += ch;
   addch(ch);
   refresh();
   break;
  }
 }
 endwin();
}

void selectSettingsAction(int option) {
 switch (option) {
  case 1:
  // Update profile
  break;
  case 2:
  // Create option
  break;
  case 3:
  system("clear");
  printMenu();
  break;
  default:
  break;
 }
}

void selectAction(int option) {
 switch (option) {
  case 1:
  system("clear");
  createNewFile("New file");
  break;
  case 2:
  cout << "hsjs";
  //fileManager.createNewDir();
  break;
  case 3:
  system("clear");
  printSettings();
  break;
  case 4:
  system("clear");
  configManager.logout();
  break;
  case 5:
  system("clear");
  break;
  case 6: {
   system("clear");
   string userInput = ioHandler.getInput < string > (
    {
     {
      "Once your account is deleted, your account will be lost forever"
     }
    },
    "Are you sure you want to delete your account? (Y/n) ", "Please select 'Y' for yes or 'n' for no"
   );
   if (userInput == "Y" || userInput == "y") {
    configManager.deleteAccount();
    system("clear");
    cout << "Account deleted successfully" << endl;
   }
   else {
    system("clear");
    printMenu();
   }
  }
  break;
  default:
  system("clear");
  exceptionHandler.printPlainError("Please select a valid option");
  printMenu();
  break;
 }
}

void printSettings() {
 for (const string& setting: settings) {
  cout << setting << endl;
 }
 int selection = ioHandler.getInput < int > ({{
  ""
 }}, "Option: ", "Please input a valid option from the list");
 if (selection < 1 || selection > 3) {
  system("clear");
  exceptionHandler.printPlainError("Select an available setting, or return to the main menu by pressing 3");
  printSettings();
  return;
 }
 selectSettingsAction(selection);
 return;
}

void printMenu() {
 userInfo = configManager.getUserInfo(false);
 if (!fileManager.navigateDir(userInfo[5])) {
  exceptionHandler.printPlainError("Please make sure you give the correct access rights to the home directory or the application will not work");
 }
 cout << "Welcome " << userInfo[1] << endl;
 cout << "**************" << endl;
 for (const string& option: options) {
  cout << option << endl;
 }
 int selection = ioHandler.getInput < int > ({{
  ""
 }}, "Option: ", "Please select a number as an option");
 if (selection < 1 || selection > 6) {
  system("clear");
  exceptionHandler.printPlainError("Please select an available option from the menu. Or you can create a new option in your settings\n");
  printMenu();
  return;
 }
 selectAction(selection);
 return;
}

void checkForAccount() {
 ifstream* config = configManager.checkForLocalConfigFile("config.yaml");
 if (!config) {
  ofstream* newConfig = configManager.createConfigFile("config.yaml");
  if (!newConfig) {
   return;
  }
  configManager.createAccount(newConfig);
  delete newConfig;
  checkForAccount();
 }
 if (config) {
  int accountEstablished = configManager.checkForExistingAccount();
  if (accountEstablished == 1) {
   return;
  }
  if (accountEstablished == 0) {
   printMenu();
   delete config;
  }
 }
}

int main(int argc, char* argv[]) {
 system("clear");
 checkForAccount();
 return 0;
}