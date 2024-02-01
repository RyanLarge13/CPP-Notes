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

void checkForAccount() {
	bool configExist = configManager.checkForLocalConfigFile();
	if (!configExist) {
		configManager.createConfigFile("config.yaml");
		configManager.createAccount();
	}
	if (configExist) {
		bool accountEstablished = configManager.checkForExistingAccount();
		//Check if user data in config
	}
}

int main(int argc, char* argv[]) {
	system("clear");
	checkForAccount();
	return 0;
}