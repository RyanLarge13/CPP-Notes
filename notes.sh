#!/usr/bin/env bash

set -e

# -----------------------------
# Configuration
# -----------------------------
CXX="clang++"
SRC_DIR="src"
BIN_DIR="bin"
OUTPUT="$BIN_DIR/myProg"

# -----------------------------
# Check compiler
# -----------------------------
if ! command -v "$CXX" &>/dev/null; then
    echo "Error: clang++ is not installed."
    exit 1
fi

# -----------------------------
# Check dependencies
# -----------------------------
if ! command -v pkg-config &>/dev/null; then
    echo "Error: pkg-config is not installed."
    exit 1
fi

if ! pkg-config --exists libcurl; then
    echo "Error: libcurl development files were not found."
    exit 1
fi

if ! pkg-config --exists libsodium; then
    echo "Error: libsodium development files were not found."
    exit 1
fi

# -----------------------------
# Prepare build directory
# -----------------------------
mkdir -p "$BIN_DIR"

# -----------------------------
# Build
# -----------------------------
echo "Building $OUTPUT..."

"$CXX" \
    -std=c++20 \
    -Wall \
    -Wextra \
    -Wpedantic \
    -g \
    -Iincludes \
    "$SRC_DIR"/*.cpp \
    -o "$OUTPUT" \
    $(pkg-config --cflags --libs libcurl libsodium) \
    -lncurses

echo "Build successful!"
echo "Run with: ./$OUTPUT"