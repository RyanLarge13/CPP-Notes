#include <unistd.h>

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "../common/exceptionHandlerInstance.h"

using namespace std;
using namespace filesystem;

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

class FileManager {
 private:
 public:
 struct DirVectorData {
  string path;
  int nestedDirCt;
  int nestedFileCt;
  DirVectorData(const string &p, int nd, int nf): path(p),
  nestedDirCt(nd),
  nestedFileCt(nf) {}
 };

 #ifdef _WIN32
 const string HOME_DIR = getenv("USERPROFILE");
 #elif defined(__APPLE__)
 const string HOME_DIR = getenv("HOME");
 #elif defined(__linux__)
 const string HOME_DIR = getenv("HOME");
 #elif defined(__ANDROID__)
 const string HOME_DIR = "/data/data/com.termux/files/home";
 #else
 const string HOME_DIR = getenv("HOME");
 #endif

 string currentDir = HOME_DIR;

 bool isCorrectExt(const directory_entry &entry) {
  string extension = entry.path().extension().string();
  if (extension == ".wn") {
   return true;
  }
  return false;
 }

 bool checkDirExists(const string &dirPath) {
  if (exists(dirPath)) {
   return true;
  }
  return false;
 }

 pair < vector < DirVectorData >,
 vector < string>> grabDirsAndFiles() {
  vector < DirVectorData > dirs;
  vector < string > files;
  path currentPath = current_path();
  for (const auto &entry: directory_iterator(currentPath)) {
   int nestedDirsCt = 0;
   int nestedFilesCt = 0;
   if (is_directory(entry.status())) {
    for (const auto &item: directory_iterator(entry.path())) {
     if (is_directory(item.status())) {
      nestedDirsCt++;
     }
     if (is_regular_file(item.status()) && isCorrectExt(item)) {
      nestedFilesCt++;
     }
    }
    dirs.emplace_back(entry.path().filename().string(), nestedDirsCt,
     nestedFilesCt);
   }
   if (is_regular_file(entry.status()) && isCorrectExt(entry)) {
    files.push_back(entry.path().filename().string());
   }
  }
  return {
   dirs,
   files
  };
 }

 string getCurrentPath() {
  path currentPath = current_path();
  return currentPath;
 }

 bool navDir(const string &dirPath) {
  if (!checkDirExists(dirPath)) {
   cout << "Does not exist returning from change dir" << endl;
   return false;
  }
  try {
   if (chdir(dirPath.c_str()) != 0) {
    cout << "Error changing dir " << strerror(errno) << endl;
    return false;
   }
  } catch (filesystem_error &err) {
   cout << "catch block err: " << err.what() << endl;
   return false;
  }
  currentDir = getCurrentPath();
  return true;
 }

 string navBack() {
  path current = getCurrentPath();
  path parent = current.parent_path();
  current_path(parent);
  currentDir = getCurrentPath();
  return current_path().filename().string();
 }

 bool navigateDir(const string &dirPath) {
  string absolutePath = HOME_DIR + dirPath;
  if (!checkDirExists(absolutePath)) {
   cout << "Does not exist returning from change dir" << endl;
   return false;
  }
  try {
   if (chdir(absolutePath.c_str()) != 0) {
    cout << "Error changing dir " << strerror(errno) << endl;
    return false;
   }
  } catch (filesystem_error &err) {
   cout << "catch block err: " << err.what() << endl;
   return false;
  }
  currentDir = getCurrentPath();
  return true;
 }

 ifstream *checkExistingFile(const string &fileName) {
  ifstream *file = new ifstream(fileName);
  if (!file->is_open()) {
   delete file;
   return nullptr;
  }
  return file;
 }

 fstream *openFileReadWrite(const string &fileName) {
  try {
   fstream *file = new fstream(fileName, ios::in | ios::out);
   if (!file->is_open()) {
    cout << "File not opened" << endl;
    cout << strerror(errno) << endl;
    delete file;
    return nullptr;
   }
   return file;
  } catch (filesystem_error &err) {
   std::cout << "Error opening" << fileName << "\n"
   << "Error: " << err.what() << "\n";
   return nullptr;
  }
 }

 ofstream *writeFile(const vector < string > &rows, ofstream *file) {
  if (!file || !file->is_open()) {
   delete file;
   return nullptr;
  }
  for (const string &row: rows) {
   *file << row << "\n";
  }
  return file;
 }

 ofstream *createNewFile(const string &fileName) {
  ofstream *newFile = new ofstream(fileName);
  if (!newFile->is_open()) {
   delete newFile;
   return nullptr;
  }
  return newFile;
 }

 bool createNewDir(const string &path) {
  try {
   if (create_directory(HOME_DIR + path)) {
    return true;
   }
   return false;
  } catch (filesystem_error &err) {
   cout << endl << "sys err " << err.what() << endl;
   return false;
  }
 }

 bool renameDir(const string &dirname, const string &newDirname) {
  bool didNavigate = navigateDir("");
  if (!didNavigate) {
   return false;
  }
  try {
   rename(dirname, newDirname);
  } catch (const filesystem_error &e) {
   cout << e.what() << endl;
   return false;
  }
  bool didNavBack = navigateDir(newDirname);
  if (!didNavBack) {
   return false;
  }
  return true;
 }

 bool deleteFile(const string &fileName) {
  if (fileName.empty()) {
   return false;
  } else {
   if (remove(fileName.c_str()) != 0) {
    return false;
   } else {
    return true;
   }
  }
 }

 bool deleteDir(const string& dirname) {
  if (dirname.empty()) {
   return false;
  }
return true;
 }

 void updateFileWeb() {}
 void deleteFileWeb() {}
 void createFileWeb() {}
};

#endif