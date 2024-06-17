#include <iostream>
#include <vector>
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
vector < string > userInfo;

const vector < string > options = {
 "1. New Note",
 "2. New Folder",
 "3. Settings",
 "4. Logout",
 "5. Delete Account"
};

void changeState(int option) {
    if (option == 1) {
        system("clear");
        // fileManager.createNewFile("New Note", ".txt");
    }
    else if (option == 2) {
        //fileManager.createNewDir();
    }
    else if (option == 3) {
        // stateManager.setState("settings");
        return;
    }
    else if (option == 4) {
        configManager.logout();
        return;
    }
    else if (option == 5) {
        system("clear");
        string userInput = ioHandler.getInput(
            {
                {"Once your account is deleted, your account will be lost forever"}
            },
            "Are you sure you want to delete your account? (Y/n) "
        );
        if (userInput == "Y" || userInput == "y") {
            configManager.deleteAccount();
            system("clear");
            cout << "Account deleted successfully" << endl;
            // Choose later if I want the user experience to be to
            // immediately recreate an account or exit the program
            // configManager.createAccount();
            return;
        }
        else {
            system("clear");
            printMenu();
        }
    }
    else {
        cout << "Please select a valid option" << endl;
        printMenu();
        return;
    }
}

void printMenu() {
    userInfo = configManager.getUserInfo(false);
    cout << "Welcome " << userInfo[1] << endl;
    cout << "**************" << endl;
    for (const string& option : options) {
        cout << option << endl;
    }
    int selection = ioHandler.getInput({{}}, "Option: ");
    int tries = 0;
    if (selection < 1 || selection > 5) {
        exceptionHandler.printPlainError("Please select an available option from the menu. Or you can create a new option in your settings");
        system("clear");
        printMenu();
    }
    changeState(selection);
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