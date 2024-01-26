#include <iostream> 
#include <limits>
using namespace std;

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

class ConfigHandler {
    private: 
	vector <string> readfile() {
		
	}
	void checkUsername() {

	}
	void checkPassword() {

	}
	void checkPin() {

	}
    public: 
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
			return;
		}
	}
	return;
}
bool checkForExistingAccount() {
	ifstream file("config.yaml");
	bool confirmed = false;
	if (!file.is_open()) {
		cout << "There was an error creating or opening a new configuration file for your account." << endl << "Would you like to manually go through the steps to fix this problem?" << endl;
		string confirmation;
		while (!confirmed) {
			cin << confirmation;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits <streamsize>::max(), '\n');
				confirmed = true;
				return false;
			}
			if (confirmation != "Y" || confirmation != "y") {
				cout << endl << "Steps to take: " << endl << "1. End this program and within your current directory type in \"touch config.yaml\" " << endl << "2. Run a command to make sure that you have read write and execution access within the directory \"chmod 777\"" << endl << "3. You are all set. Restart the application and try again";
				confirmed = true;
				return false;
			}
		}
	}
	string line;
	string value;
	getline(file, line);
	stringstream ss(line);
	vector <string> rows;
	while(getline(ss, value, '\n')) {
		row.push_back(value);
	}
	for (const string& row: rows) {
		cout << row << endl;
	}
	if (rows.size() < 1) {
		return false;
	}
	return false;
}
};

#endif