#ifndef LOCALAUTH_H
#define LOCALAUTH_H
#include <iostream>
using namespace std;

class LocalAuth {
    private:
    bool compare() {
        
    }
    string encrypt(string hash) {

    }
    bool decrypt(string hash) {

    }
    public:
    bool checkPassword(string password) {
        string hash = "@$$#^WFREQr3jfoid9fenwqifiejq3@repogqwpjfwTKREQJKREW";
        bool passwordMatch = decrypt(password, hash);
        passwordMatch ? true : false;
    }

};

#endif