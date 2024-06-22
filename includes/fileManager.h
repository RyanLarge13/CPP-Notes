#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <string>
#include <vector>
#include <filesystem>
#include "../common/exceptionHandlerInstance.h"
using namespace std;
using namespace filesystem;

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

class FileManager {
 private:
 public:

 void closeFile(fstream* file) {
  file->close();
 }

 bool checkDirExists(const string& dirPath) {
  if (exists(dirPath)) {
   return true;
  }
  return false;
 }

 bool navigateDir(const string& dirPath) {
  if (!checkDirExists(dirPath)) {
   return false;
  }
  if (!chdir(dirPath.c_str())) {
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
   if (create_directory(path) && !checkDirExists(path)) {
    return true;
   }
   return false;
  }
  catch (filesystem_error& err) {
   cout << "sys err " << endl;
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