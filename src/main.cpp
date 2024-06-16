#include <iostream>
#include <vector>
#include <limits>
#include <filesystem>
#include <fstream>
#include <limits>
#include "../includes/fileManager.h"
#include "../includes/configManager.h"
#include "../includes/httpHandler.h"
#include "../includes/stateManager.h"

using namespace std;
namespace fs = filesystem;

FileManager fileManager;
ConfigManager configManager;
StateManager stateManager;

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
        cout << "Are you sure you want to delete your account?" << endl
            << "Once deleted, your account will be lost forever" << endl;
        int selection;
        bool select = true;
        while (select) {
            cout << "Cancel: 0, Delete: 1: ";
            cin >> selection;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits < streamsize > ::max());
                cout << endl << "Please select a valid option" << endl;
            }
            if (selection == 0) {
                cout << endl << "Deleting" << endl;
                configManager.deleteAccount();
                system("clear");
                configManager.createAccount();
                select = false;
            }
            if (selection == 1) {
                system("clear");
                printMenu();
                select = false;
            }
        }
    }
    // else if (option == 10) {}
    else {
        cout << "Please select a valid option" << endl;
        printMenu();
        return;
    }
}

void printMenu() {
    bool getOption = true;
    userInfo = configManager.getUserInfo(false);
    cout << "Welcome " << userInfo[1] << endl;
    cout << "**************" << endl;
    for (string option : options) {
        cout << option << endl;
    }
    int selection;
    int tries = 0;
    while (getOption) {
        cout << endl << "Option: ";
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
        else if (selection < 1 || selection > 5) {
            cout << "Please select an available option from the menu. You can create options in your settings" << endl;
        }
        else {
            getOption = false;
        }
    }
    changeState(selection);
}

void checkForAccount() {
    bool configExist = configManager.checkForLocalConfigFile();
    if (!configExist) {
        configManager.createConfigFile("config.yaml");
        configManager.createAccount();
        checkForAccount();
    }
    if (configExist) {
        int accountEstablished = configManager.checkForExistingAccount();
        if (accountEstablished == 1) {
            return;
        }
        if (accountEstablished == 0) {
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