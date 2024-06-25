#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>
#include "../common/exceptionHandlerInstance.h"
using namespace std;
using namespace filesystem;

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#ifdef _WIN32
const std::string HOME_DIR = std::getenv("USERPROFILE");
#elif defined(__APPLE__)
const std::string HOME_DIR = std::getenv("HOME");
#elif defined(__linux__)
const std::string HOME_DIR = std::getenv("HOME");
#elif defined(__ANDROID__)
const std::string HOME_DIR = "/data/data/com.termux/files/home";
#else
const std::string HOME_DIR = std::getenv("HOME");
#endif

class FileManager {
 private:

 public:
static struct DirVetorData {
    string path;
    int nestedDirectories;
    int nestedFiles;
    DirVectorData(const string& p, int nd, int nf): path(p), nestedDirectories(nd), nestedFiles(nf) {}
};

 bool isCorrectExt(auto& entry) {
    string extension = entry.path().extension().string();
    if (extension == ".wn") {
        return true;
    }
    return false;
 }

 void closeFile(fstream* file) {
  file->close();
 }

 bool checkDirExists(const string& dirPath) {
  if (exists(dirPath)) {
   return true;
  }
  return false;
 }

 vector <vector <DirVectorData>, vector<string>> grabDirsAndFiles() {
    vector<DirVectorData> dirs;
    vector<string> files;
    path currentPath = current_path();
    for (const auto& entry : directory_iterator(currentPath)) {
        int nestedDirsCt = 0;
        int nestedFilesCt = 0;
        if (is_directory(entry.status())) {
            for (const auto& item : directory_iterator(entry.path())) {
                if (is_directory(item.status())) {
                    nestedDirsCt++;
                }
                if (is_regular_file(item.status()) && isCorrectExt(item)) {
                    nestedFilesCt++;
                }
            }
            dirs.emplace_back(entry.path().string(), nestedDirsCt, nestedFilesCt);
        }
        if (is_regular_file(entry.status()) && isCorrectExt(entry)) {
            files.push_back(entry.filename().string());
        }
    }
    return {dirs, files};
 }

 void printFiles() {

 }

 bool navigateDir(const string& dirPath) {
  string absolutePath = HOME_DIR + dirPath;
  if (!checkDirExists(absolutePath)) {
   cout << "path does not exist" << endl << HOME_DIR + dirPath << endl;
   return false;
  }
  try {
   if (chdir(absolutePath.c_str()) != 0) {
    cout << "Error changing dir " << strerror(errno) << endl;
    return false;
   }
  } catch (filesystem_error& err) {
   cout << "catch block err: " << err.what() << endl;
   return false;
  }
  return true;
 }

 ifstream* checkExistingFile(const string& fileName) {
  ifstream* file = new ifstream(fileName);
  if (!file->is_open()) {
   delete file;
   return nullptr;
  }
  return file;
 }

 fstream* openFileReadWrite(const string& fileName) {
  fstream* file = new fstream(fileName);
  if (!file->is_open()) {
   delete file;
   return nullptr;
  }
  return file;
 }

 ofstream* writeFile(const vector < string>& rows, ofstream* file) {
  if (!file || !file->is_open()) {
   delete file;
   return nullptr;
  }
  for (const string& row: rows) {
   *file << row << "\n";
  }
  return file;
 }

 ofstream* createNewFile(const string& fileName) {
  ofstream* newFile = new ofstream(fileName);
  if (!newFile->is_open()) {
   delete newFile;
   return nullptr;
  }
  return newFile;
 }

 bool createNewDir(const string& path) {
  try {
   if (create_directory(HOME_DIR + path)) {
    return true;
   }
   return false;
  }
  catch (filesystem_error& err) {
   cout << endl << "sys err " << err.what() << endl;
   return false;
  }
 }

 bool deleteFile(const string& fileName) {
  if (fileName.empty()) {
   return false;
  }
  else {
   if (remove(fileName.c_str()) != 0) {
    return false;
   }
   else {
    return true;
   }
  }
 }

 void updateFileWeb() {}
 void deleteFileWeb() {}
 void createFileWeb() {}
};

#endif