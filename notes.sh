#!bin/bash

if command -v clang &>/bin/null; then
    echo clang++ -o bin/myProg src/main.cpp -lncurses -lcurl
else
    echo "Please install clang"
fi
