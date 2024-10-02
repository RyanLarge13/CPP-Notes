#include <ncurses.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <vector>

#include "../includes/colors.h"
#include "../includes/configManager.h"
#include "../includes/exceptionHandler.h"
#include "../includes/fileManager.h"
#include "../includes/httpHandler.h"
#include "../includes/ioHandler.h"
#include "../includes/stateManager.h"

using namespace std;
namespace fs = filesystem;

FileManager fileManager;
ConfigManager configManager;
StateManager stateManager;
ExceptionHandler exceptionHandler;
IoHandler ioHandler;

void printMenu();
void printSettings();
pair<vector<FileManager::DirVectorData>, vector<string>> dirInfo;
vector<string> userInfo;
int nesting = 0;
string title = "HOME";

enum SettingsOptions : int { USERNAME = 1, PASSWORD = 2, PIN = 3, DIR = 4 };

const vector<string> options = {"1. New Note",
                                "2. Open Note",
                                "3. New Folder",
                                "4. Open Folder",
                                "5. Settings",
                                "6. Logout",
                                "7. Quit (stay logged in)",
                                "8. Delete Account"};

const vector<string> settings = {"1. Update profile", "2. Create option",
                                 "3. Main menu", "4. Delete Account"};

void createOption() {
  string newOption =
      ioHandler.getInput<string>({{""}}, "What is your new option name: ",
                                 "Please provide a valid name with no spaces");
  int optionIndex;
}

void printDirs(const bool &showIndex, const int &start) {
  int iterator = start;
  auto &dirs = dirInfo.first;
  if (dirs.size() < 1) {
    return;
  }
  for (const auto &dir : dirs) {
    if (showIndex) {
      cout << BLUE + "#" + to_string(iterator) + ENDCOLOR << " ";
      iterator++;
    }
    cout << RED + dir.path + ENDCOLOR << " ";
    cout << YELLOW << dir.nestedDirCt << ENDCOLOR << " ";
    cout << BLUE << dir.nestedFileCt << ENDCOLOR << "    ";
  }
  cout << endl;
}

void printFiles(const bool &showIndex, const int &start) {
  int iterator = start;
  auto &files = dirInfo.second;
  if (files.size() < 1) {
    return;
  }
  for (const auto &file : files) {
    if (showIndex) {
      cout << PURPLE + "#" + to_string(iterator) + ENDCOLOR << " ";
      iterator++;
    }
    cout << BLUE + file + ENDCOLOR << "    ";
  }
  cout << endl;
}

void createNewFile() {
  string fileName = ioHandler.getInput<string>(
      {{"Give your new note a name"}},
      "New name: ", "Please provide the file with a valid name");
  ofstream *newFile = fileManager.createNewFile(fileName);
  if (!newFile) {
    exceptionHandler.printPlainError(
        "Please make sure you have the necessary read write permissions set to "
        "your main directory" +
        userInfo[6] + "We could not create your new note");
    delete newFile;
    printMenu();
  }
  // grab all available directories the user can select from to save the new
  // note in
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
  int ch;
  int x;
  int y;
  string text;
  // printw(fileName + "\n\n");
  move(1, 0);
  while ((ch = getch()) != 27) {
    switch (ch) {
    case KEY_BACKSPACE:
    case 127: {
      if (!text.empty()) {
        text.pop_back();
        getyx(stdscr, y, x);
        if (x > 0)
          x--;
        if (y > 1) {
          y--;
          x = getmaxx(stdscr) - 1;
        }
        mvdelch(y, x);
        refresh();
      }
    } break;
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
  for (char textCh : text) {
    *newFile << textCh;
  }
  newFile->close();
  delete newFile;
  cout << "Your new note " << fileName << " was successfully saved" << endl;
  printMenu();
}

void createNewDir() {
  string newDirName = ioHandler.getInput<string>(
      {{"Give your new folder a name"}},
      "New folder: ", "Please provide a valid directory name");
  // grab all available directories the user can select from to save the new
  // directory in
  bool dirCreated = fileManager.createNewDir(userInfo[5] + "/" + newDirName);
  if (!dirCreated) {
    exceptionHandler.printPlainError(
        "We could not create your new folder. Please make sure you have the "
        "correct access rights set in your main directory " +
        userInfo[6]);
    printMenu();
    return;
  }
  const auto &newDirInfo = fileManager.grabDirsAndFiles();
  dirInfo = newDirInfo;
  system("clear");
  cout << "New folder " << newDirName << " created" << endl;
  printMenu();
}

void initializeTextEditor(const string &text) {
  if (text.empty()) {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    int ch;
    int x;
    int y;
    string text;
    // printw(fileName + "\n\n");
    move(1, 0);
    while ((ch = getch()) != 27) {
      switch (ch) {
      case KEY_BACKSPACE:
      case 127: {
        if (!text.empty()) {
          text.pop_back();
          getyx(stdscr, y, x);
          if (x > 0)
            x--;
          if (y > 1) {
            y--;
            x = getmaxx(stdscr) - 1;
          }
          mvdelch(y, x);
          refresh();
        }
      } break;
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
  } else {
  }
}

void openNote() {
  if (dirInfo.second.size() < 1) {
    exceptionHandler.printPlainError("You have no notes. Create one!!");
    printMenu();
    return;
  }
  printFiles(true, 0);
  int selection = ioHandler.getInput<int>(
      {{""}}, "Note #: ", "Pleas provide a valid input");
  if (selection < 0 || selection > dirInfo.second.size()) {
    exceptionHandler.printPlainError(
        "Please select a valid note to navigate to or type 999 to return to "
        "menu");
  }
  string fileStr = dirInfo.second[selection];
  fstream *note = fileManager.openFileReadWrite(fileStr);
  if (!note) {
    exceptionHandler.printPlainError(
        "We could not open your note. Please make sure you have permissions "
        "set correctly on your file");
    printMenu();
    delete note;
  }
  string value;
  string line;
  while (getline(*note, line)) {
    value += line;
  }
  cout << value << endl << endl;
  char action =
      ioHandler.getInput<char>({{"Type 'Q' to quit"}, {"Type E to edit"}},
                               "Command: ", "Please provide a valid option");
  if (action == 'E') {
    initializeTextEditor(value);
  } else {
    system("clear");
    delete note;
    printMenu();
  }
}

void openDir() {
  if (dirInfo.first.size() < 1 && nesting == 0) {
    exceptionHandler.printPlainError("You have no folders. Create one!!");
    printMenu();
    return;
  }
  printDirs(true, 0);
  int selection = ioHandler.getInput<int>(
      {{"Navigate back with 999", "Select a folder index to navigate"}},
      "Folder #: ", "Please provide a valid input");
  int foldersLength = dirInfo.first.size();
  if (selection < 0 || selection > foldersLength && selection != 999) {
    exceptionHandler.printPlainError("Please select a valid folder to navigate "
                                     "to or type 999 to navigate back");
  }
  if (selection == 999) {
    if (nesting > 0) {
      system("clear");
      string backDirName = fileManager.navBack();
      const auto &newDirData = fileManager.grabDirsAndFiles();
      dirInfo = newDirData;
      if ("/" + backDirName == userInfo[5]) {
        title = "HOME";
      } else {
        title = backDirName;
      }
      nesting--;
      printMenu();
      return;
    }
    if (nesting == 0) {
      system("clear");
      exceptionHandler.printPlainError("You are already home");
      openDir();
    }
  }
  string currentPath = fileManager.getCurrentPath();
  string folderString = dirInfo.first[selection].path;
  string newDirPath = currentPath + "/" + folderString;
  fileManager.navDir(newDirPath);
  const auto &newDirInfo = fileManager.grabDirsAndFiles();
  nesting++;
  title = folderString;
  dirInfo = newDirInfo;
  system("clear");
  printMenu();
  return;
}

void updateProfile() {
  vector<string> updateOptions = {"1. Change username", "2. Change password",
                                  "3. Change pin", "4. Set main directory"};
  for (const string &option : updateOptions) {
    cout << option << endl;
  }
  cout << endl;
  int optionPicked = ioHandler.getInput<int>(
      {{""}}, "Select your option: ", "Please provide a valid option to 1 - 4");
  if (optionPicked < 1 || optionPicked > 4) {
    system("clear");
    cout << "Please provide a valid option 1 - 4" << endl;
    updateProfile();
  }
  switch (optionPicked) {
  case SettingsOptions::USERNAME:
    system("clear");
    configManager.changeUsername(userInfo);
    printMenu();
    break;
  case SettingsOptions::PASSWORD:
    configManager.changePass(userInfo);
    printMenu();
    break;
  case SettingsOptions::PIN:
    configManager.changePin(userInfo);
    printMenu();
    break;
  case SettingsOptions::DIR:
    configManager.changeDir(userInfo);
    printMenu();
    break;
  }
}

void selectSettingsAction(int option) {
  switch (option) {
  case 1:
    system("clear");
    updateProfile();
    break;
  case 2:
    system("clear");
    createOption();
    break;
  case 3:
    system("clear");
    printMenu();
    break;
  case 4: {
    system("clear");
    string userInput = ioHandler.getInput<string>(
        {{"Once your account is deleted, your account will be lost forever"}},
        "Are you sure you want to delete your account? (Y/n) ",
        "Please select 'Y' for yes or 'n' for no");
    if (userInput == "Y" || userInput == "y") {
      configManager.deleteAccount();
      system("clear");
      cout << "Account deleted successfully" << endl;
    } else {
      system("clear");
      printMenu();
    }
  } break;
  default:
    break;
  }
}

void selectAction(int option) {
  switch (option) {
  case 1:
    system("clear");
    createNewFile();
    break;
  case 2:
    openNote();
    break;
  case 3:
    createNewDir();
    break;
  case 4:
    system("clear");
    openDir();
    break;
  case 5:
    system("clear");
    printSettings();
    break;
  case 6:
    system("clear");
    configManager.logout();
    break;
  case 7:
    system("clear");
    break;
  default:
    system("clear");
    exceptionHandler.printPlainError("Please select a valid option");
    printMenu();
    break;
  }
}

void printSettings() {
  for (const string &setting : settings) {
    cout << setting << endl;
  }
  int selection = ioHandler.getInput<int>(
      {{""}}, "Option: ", "Please input a valid option from the list");
  if (selection < 1 || selection > 3) {
    system("clear");
    exceptionHandler.printPlainError(
        "Select an available setting, or return to the main menu by pressing "
        "3");
    printSettings();
    return;
  }
  selectSettingsAction(selection);
  return;
}

void printMenu() {
  cout << "Folder: " << RED + fileManager.getCurrentPath() + ENDCOLOR << " "
       << endl
       << endl;
  cout << title << endl << endl;
  cout << "Welcome " << userInfo[1] << endl;
  cout << "**************" << endl;
  for (const string &option : options) {
    cout << option << endl;
  }
  cout << endl << endl;
  printDirs(false, 0);
  printFiles(false, 0);
  int selection = ioHandler.getInput<int>(
      {{""}}, "Option: ", "Please select a number as an option");
  if (selection < 1 || selection > 7) {
    system("clear");
    exceptionHandler.printPlainError(
        "Please select an available option from the menu. Or you can create a "
        "new option in your settings\n");
    printMenu();
    return;
  }
  selectAction(selection);
  return;
}

void checkForAccount() {
  ifstream *config = configManager.checkForLocalConfigFile(
      fileManager.HOME_DIR + "/" + "config.yaml");
  if (!config) {
    delete config;
    ofstream *newConfig = configManager.createConfigFile(fileManager.HOME_DIR +
                                                         "/" + "config.yaml");
    if (!newConfig) {
      delete newConfig;
      return;
    }
    configManager.createAccount(newConfig);
    newConfig->close();
    delete newConfig;
    checkForAccount();
  }
  if (config) {
    int accountEstablished = configManager.checkForExistingAccount();
    if (accountEstablished == 1) {
      vector<string> currentData = configManager.getUserInfo(false);
      configManager.finishCreatingAccount(currentData);
      config->close();
      delete config;
      checkForAccount();
    }
    if (accountEstablished == 0) {
      userInfo = configManager.getUserInfo(false);
      if (!fileManager.navigateDir(userInfo[5])) {
        // Make sure you print instructions on how to manually fix this problem
        // before returning out of the program with
        // exceptionHandler.printInstructions()
        exceptionHandler.printPlainError(
            "Please make sure you give the correct access rights to the home "
            "directory or the application will not work");
      }
      const auto &info = fileManager.grabDirsAndFiles();
      dirInfo = info;
      config->close();
      delete config;
      printMenu();
    }
  }
}

int main(int argc, char *argv[]) {
  system("clear");
  checkForAccount();
  return 0;
}