#include <iostream>
#include <vector>
#include <limits>
using namespace std;

void createAccount() {

}

void printMenu() {
    cout << "*** Welcome to CPP Notes ***" << endl << endl;
    vector <string> options;
    options.push_back("1. Create account");
    options.push_back("2. Exit");
    int selectedOption;
    for (const auto& pair : options) {
        cout << pair << endl;
    }
    cout << endl << "Option: ";
    cin >> selectedOption;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        system("clear");
        cout << endl << "You must pick a valid option from the menu" << endl;
        printMenu();
        return;
    }
    if (selectedOption > 2 || selectedOption < 1) {
        system("clear");
        cout << endl << "You must pick a valid option from the menu" << endl;
        printMenu();
        return;
    }
    switch (selectedOption) {
    case 1:
        createAccount();
        break;
    case 2:
        return;
        break;
    }
}

int main(int argc, char* argv[]) {
    system("clear");
    printMenu();
    return 0;
}