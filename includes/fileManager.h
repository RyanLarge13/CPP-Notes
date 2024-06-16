#include <iostream>
#include <cstdio> // for remove
#include <string>
#include <vector>
#include <filesystem>
using namespace std;
using namespace filesystem;

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

class FileManager {
private:
    void determineDir() {}
public:

    void closeFile(fstream* file) {
        file->close();
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
            cout << "Could not open file: " << fileName << endl;
            delete file;
            return nullptr;
        }
        return file;
    }

    ofstream* writeFile(const vector<string>& rows, ofstream* file) {
        if (!file || !file->is_open()) {
            cout << "No file to write to" << endl;
            delete file;
            return nullptr;
        }
        for (const string& row : rows) {
            *file << row << "\n";
        }
        return file;
    }

    ofstream* createNewFile(const string& fileName) {
        ofstream* newFile = new ofstream(fileName);
        if (!newFile->is_open()) {
            cout << "Could not create file" << endl;
            delete newFile;
            return nullptr;
        }
        return newFile;
    }

    void createNewDir(const string& path) {
        try {
            if (create_directory(path)) {
                return;
            }
            else {
                cout << "This directory already exists, please choose a different name or create this directory in a different directory" << endl;
                return;
            }
        }
        catch (filesystem_error& err) {
            cout << "There was a problem creating your new directory. Please try again" << endl;
            return;
        }
    }

    bool deleteFile(const string& fileName) {
        if (fileName.empty()) {
            cout << "Please provide a file name to delete" << endl;
            return false;
        }
        else {
            if (remove(fileName.c_str()) != 0) {
                cout << "Error deleting file" << endl;
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