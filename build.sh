#!/bin/bash
# Honey Encryption - Build Script for WSL/Linux

echo "================================"
echo "Honey Encryption - Build Script"
echo "================================"

# Check for OpenSSL
if ! command -v openssl &> /dev/null; then
    echo "ERROR: OpenSSL not found."
    echo "Install with: sudo apt-get install libssl-dev"
    exit 1
fi

# Check for g++ or clang++
if ! command -v g++ &> /dev/null; then
    echo "ERROR: g++ not found."
    echo "Install with: sudo apt-get install g++"
    exit 1
fi

echo "Found g++ and OpenSSL, compiling..."

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build
make

if [ $? -eq 0 ]; then
    echo ""
    echo "================================"
    echo "Build successful!"
    echo "Run: ./honey_encrypt"
    echo "================================"
else
    echo "Build failed!"
    exit 1
fi

