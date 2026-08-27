#!bin/bash

if command -v clang &>/bin/null; then
    echo clang++ src/main.cpp -o bin/myProg \
    $(pkg-config --cflags --libs libcurl) \
    -lncurses
else
    echo "Please install clang"
fi
