#include <iostream>
#include <vector>
#include <ncurses.h>
#include <limits>
#include <filesystem>
#include <fstream>
#include <limits>
#include "../includes/colors.h"
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
pair <vector<FileManager::DirVectorData>, vector <string>> dirInfo;
vector < string > userInfo;

const vector < string > options = {
 "1. New Note",
 "2. Open Note",
 "3. New Folder",
 "4. Open Folder",
 "5. Settings",
 "6. Logout",
 "7. Quit (stay logged in)",
 "8. Delete Account"
};

const vector < string > settings = {
 "1. Update profile",
 "2. Create option",
 "3. Main menu",
 "4. Delete Account"
};

void printDirs(const bool& showIndex, const int& start) {
    int iterator = start;
    auto& dirs = dirInfo.first;
    if (dirs.size() < 1) {
        cout << "no len folders" << endl;
        return;
    }
    for (const auto& dir : dirs) {
        if (showIndex) {
            cout << BLUE + to_string(iterator) + ENDCOLOR;
        }
        cout << RED + dir.path + ENDCOLOR << " ";
        cout << YELLOW << dir.nestedDirCt << ENDCOLOR << " ";
        cout << BLUE << dir.nestedFileCt << ENDCOLOR << " ";
        if (showIndex) {
            iterator++;
        }
    }
    cout << endl;
}

void printFiles(const bool& showIndex, const int& start) {
    int iterator = start;
    auto& files = dirInfo.second;
    if (files.size() < 1) {
        cout << "no len files" << endl;
        return;
    }
    for (const auto& file : files) {
        if (showIndex) {
            cout << PURPLE + to_string(iterator) + ENDCOLOR;
        }
        cout << BLUE + file + ENDCOLOR << " ";
        if (showIndex) {
            iterator++;
        }
    }
    cout << endl;
}

void createNewFile() {
    string fileName = ioHandler.getInput < string >({ {
     "Give your new note a name"
    } }, "New name: ", "Please provide the file with a valid name");
    ofstream* newFile = fileManager.createNewFile(fileName);
    if (!newFile) {
        exceptionHandler.printPlainError("Please make sure you have the necessary read write permissions set to your main directory" + userInfo[6] + "We could not create your new note");
        delete newFile;
        printMenu();
    }
    // grab all available directories the user can select from to save the new note in
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
        case 127:
        {
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
    for (char textCh : text) {
        *newFile << textCh;
    }
    newFile->close();
    delete newFile;
    cout << "Your new note " << fileName << " was successfully saved" << endl;
    printMenu();
}

void createNewDir() {
    string newDirName = ioHandler.getInput < string >({ {
     "Give your new folder a name"
    } }, "New folder: ", "Please provide a valid directory name");
    // grab all available directories the user can select from to save the new directory in
    bool dirCreated = fileManager.createNewDir(newDirName);
    if (!dirCreated) {
        exceptionHandler.printPlainError("We could not create your new folder. Please make sure you have the correct access rights set in your main directory " + userInfo[6]);
        printMenu();
        return;
    }
    cout << "New folder " << newDirName << " created" << endl;
    printMenu();
}

void initializeTextEditor(const string& text) {
    if (text.empty()) {
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
            case 127:
            {
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
    else {}
}

void openNote() {
    string noteString = ioHandler.getInput < string >({ {
     "Open note"
    } }, "Name of file: ", "Please input a valid file name");
    fstream* note = fileManager.openFileReadWrite(noteString);
    if (!note) {
        exceptionHandler.printPlainError("We could not open your note. Please make sure you have permissions set correctly on your file");
        printMenu();
        delete note;
    }
    string value;
    string line;
    while (getline(*note, line)) {
        value += line;
    }
    cout << value << endl << endl;
    char action = ioHandler.getInput < char >({ {
     "Type 'Q' to quit"
    }, {
     "Type E to edit"
    } }, "Command: ", "Please provide a valid option");
    if (action == 'E') {
        initializeTextEditor(value);
    }
    else {
        system("clear");
        delete note;
        printMenu();
    }
}

void openDir() {
    system("clear");
    printDirs(true, 0);
    int selection = ioHandler.getInput < int >({ {
     ""
    } }, "Folder #: ", "Please provide a valid input");
    int foldersLength = dirInfo.first.size();
    if (selection < 0 || selection > foldersLength && selection != 999) {
        exceptionHandler.printPlainError("Please select a valid folder to navigate to or type 999 to return to menu");
    }
    if (selection == 999) {
        system("clear");
        printMenu();
        return;
    }
    string folderString = dirInfo.first[selection].path;
    fileManager.navigateDir(folderString);
    system("clear");
    cout << "Folder: " + RED + folderString + ENDCOLOR << endl << endl;
    printMenu();
    return;
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
    case 4:
    {
        system("clear");
        string userInput = ioHandler.getInput < string >(
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
    for (const string& setting : settings) {
        cout << setting << endl;
    }
    int selection = ioHandler.getInput < int >({ {
     ""
    } }, "Option: ", "Please input a valid option from the list");
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
    // Make sure you uncomment this at some point and implement these checks
    // bool mainNotesDirExists = fileManager.checkDirExists(userInfo[5]);
    // if (!mainNotesDirExists) {
    //     exceptionHandler.printPlainError("It seems you have either moved or deleted your notes directory. We are creating a new one new");
    //     bool dirCreated = configManager.nameMainDir(userInfo[5]);
    //     if (!dirCreated) {
    //         // Make sure you print instructions on how to manually fix this problem before returning out of the program with exceptionHandler.printInstructions()
    //         exceptionHandler.printPlainError("Please make sure you give the correct access rights to the home directory or the application will not work");
    //         return;
    //     }
    // }
    if (!fileManager.navigateDir(userInfo[5])) {
        // Make sure you print instructions on how to manually fix this problem before returning out of the program with exceptionHandler.printInstructions()
        exceptionHandler.printPlainError("Please make sure you give the correct access rights to the home directory or the application will not work");
    }
    cout << "Folder: " << RED + userInfo[5] + ENDCOLOR << " " << endl << endl;
    cout << "Welcome " << userInfo[1] << endl;
    cout << "**************" << endl;
    for (const string& option : options) {
        cout << option << endl;
    }
    cout << endl << endl;
    printDirs(false, 0);
    printFiles(false, 0);
    int selection = ioHandler.getInput < int >({ {
     ""
    } }, "Option: ", "Please select a number as an option");
    if (selection < 1 || selection > 7) {
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
        delete config;
        ofstream* newConfig = configManager.createConfigFile("config.yaml");
        if (!newConfig) {
            delete newConfig;
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
            const auto& dirInfo = fileManager.grabDirsAndFiles();
            printMenu();
        }
        delete config;
    }
}

int main(int argc, char* argv[]) {
    system("clear");
    checkForAccount();
    return 0;
}