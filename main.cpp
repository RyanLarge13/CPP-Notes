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

void printMenu() {
	vector <string> userInfo = configManager.getUserInfo();
	cout << "Welcome " << userInfo[0] << endl;
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