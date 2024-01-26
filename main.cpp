#include <iostream>
#include <vector>
#include <limits>
#include <filesystem>
#include <fstream>
#include <limits>
#include "./includes/filemanager.h"
using namespace std;
namespace fs = std::filesystem;

FileManager fileManager;

void initializeAccount() {
	bool getUsername = true;
	bool getPassword = true;
	bool getPasswordConfirm = true;
	bool getPin = false;
	string username;
	string password;
	string passwordConfirm;
	int pin;
	cout << "Lets create a new account for you so you can sync your notes and keep your data safe." << endl << endl;
	cout << "What would you like your new username to be? ";
	while (getUsername) {
		cin >> username;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits < streamsize > ::max(), '\n');
			cout << "Please enter a valid username" << endl << "This username should be at minimum 4 characters" << endl << "Your new username can only include:" << endl << "- A-Z, a-z, numbers, _, and -" << endl;
		}
		if (username.length() < 4) {
			cout << endl << "Your username must be at minimum 4 characters long" << endl;
		}
		getUsername = false;
	}
	cout << "Great choice on username" << endl;
	cout << "You can always change your username later" << endl;
	cout << "Input a password" << endl;
	while (getPassword) {
		cin >> password;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits < streamsize > ::max(), '\n');
			cout << "Please enter a valid password" << endl << "This username should be at minimum 8 characters" << endl << "Your new password must include:" << endl << "2 uppercase letters" <<
			endl << "2 lowercase letters" <<
			endl << "At least 2 numbers" <<
			endl << "At least two special characters" <<
			endl << "And be a total of 8 characters in length" << endl;
		}
		if (password.length() < 8) {
			cout << endl << "Your new password must be at least 8 characters long" << endl;
			return;
		}
		getPassword = false;
	}
	cout << "Confirm your password: ";
	while (getPasswordConfirm) {
		cin >> getPasswordConfirm;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits < streamsize > ::max(), '\n');
			cout << "Please enter a valid username" << endl << "This username should be at minimum 4 characters" << endl << "Your new username can only include:" << endl << "- A-Z, a-z, numbers, _, and -" << endl;
		}
	}
}

void checkForNotesDirectory() {
	string relativePath = "my_notes";
	fs::path absolutePath = fs::absolute(relativePath);
	if (!fs::exists(absolutePath) || !fs::is_directory(absolutePath)) {
		try {
			fs::create_directory(absolutePath);
			return;
		}
		catch (const std::filesystem::filesystem_error& e) {
			char answer;
			cout << e.what() << endl << "There was a problem creating a directory to store your new notes when using this application" << endl << "Would you like to continue and create this directory manually?" << endl << "(Y/N)Yes/No: ";
			cin >> answer;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits < streamsize > ::max());
				return;
			}
			if (answer == 'Y' || answer == 'y') {
				cout << endl << "Steps to take: " << endl << "1. End this program and within your current directory type in \"mkdir my_notes\" " << endl << "2. Run a command to make sure that you have read write and execution access within the directory \"chmod 777\"" << endl << "3. You are all set. Restart the application and try again";
				return;
			}
			return;
		}
		return;
	}
	return;
}

void createConfigFile(string filename) {
	ofstream newFile(filename);
	if (!newFile.is_open()) {
		char answer;
		cout << endl << "We are having issues initializing a configuration file for your new account. Would you like to create this configuration file manually?" << endl << " (Y/N) Yes/No: ";
		cin >> answer;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits < streamsize > ::max());
			return;
		}
		if (answer == 'Y' || answer == 'y') {
			cout << endl << "Steps to take: " << endl << "1. End this program and within your current directory type in \"touch config.yaml\" " << endl << "2. Run a command to make sure that you have read write and execution access within the directory \"chmod 777 config.yaml\"" << endl << "3. You are all set. Restart the application and try again";
		}
		return;
	}
	ifstream file(newFile);
	if (!file.is_open()) {
		
	}
	return;
}

bool checkForLocalConfigFile() {
	string filename = "config.yaml";
	ifstream file(filename);
	if (!file.is_open()) {
		return false;
	}
	return true;
}

void printMenu() {
	checkForNotesDirectory();
	/*	bool configFileExists = checkForLocalUserAndConfigFile();
	if (!configFileExists) {
		createConfigFile("config.yaml");
		initializeAccount();
	}*/
	cout << "*** Welcome to CPP Notes ***" << endl << endl;
	vector < string > options;
	options.push_back("1. Create account");
	options.push_back("2. Exit");
	int selectedOption;
	for (const auto& pair: options) {
		cout << pair << endl;
	}
	cout << endl << "Option: ";
	cin >> selectedOption;
	if (cin.fail()) {
		cin.clear();
		cin.ignore(numeric_limits < streamsize > ::max(), '\n');
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
		return;
		break;
		case 2:
		return;
		break;
	}
}

void checkForAccount() {
	bool configExist = checkForLocalConfigFile();
	if (!configExist) {}
	if (configExist) {}
}

int main(int argc, char* argv[]) {
	system("clear");
	checkForAccount();
	//	printMenu();
	return 0;
}