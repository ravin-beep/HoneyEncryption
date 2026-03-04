# Honey Encryption System

A C++ implementation of Honey Encryption with Fake Plaintext Generation, configured for WSL (Windows Subsystem for Linux).

## Project Overview

Honey Encryption is a security technique that returns plausible-looking fake plaintexts (honeywords) when decryption fails or wrong keys are used. This prevents attackers from knowing whether their decryption attempts were successful.

**Security Principle:** No correctness signal is leaked to the attacker.

## Features

- **AES-256-CBC Encryption** via OpenSSL
- **SHA-256 Key Derivation** from password
- **Distribution Transforming Encoder (DTE)** for seed mapping
- **Two Fake Plaintext Generation Strategies:**
  1. Frequency-based credentials (USERNAME/PASSWORD/EMAIL format)
  2. Markov chain text generator
- **No Correctness Signal** - every decryption returns valid output

## Project Structure

```
HoneyEncryption/
├── main.cpp              # CLI menu system
├── aes_module.h/cpp      # AES-256-CBC encryption
├── dte_module.h/cpp       # Distribution Transforming Encoder
├── fake_generator.h/cpp   # Fake plaintext generation
├── utils.h/cpp           # Utility functions (hex encoding, etc.)
├── CMakeLists.txt        # CMake build configuration
├── build.sh              # Linux/WSL build script
└── README.md             # This file
```

## Prerequisites (WSL)

1. **Install OpenSSL development libraries:**
   ```bash
   sudo apt-get update
   sudo apt-get install libssl-dev
   ```

2. **Install g++ (if not already installed):**
   ```bash
   sudo apt-get install g++
   ```

3. **Install CMake (optional, for CMake build):**
   ```bash
   sudo apt-get install cmake
   ```

## Building on WSL

### Option 1: Using the build script
```bash
chmod +x build.sh
./build.sh
```

### Option 2: Using CMake
```bash
mkdir -p build
cd build
cmake ..
make
```

### Option 3: Direct compilation
```bash
g++ -o honey_encrypt -I. main.cpp aes_module.cpp dte_module.cpp fake_generator.cpp utils.cpp -lssl -lcrypto
```

## Running

After building, run:
```bash
./honey_encrypt
```

## Usage

The program provides a menu-based interface:

1. **Encrypt Message** - Enter plaintext and password to encrypt
2. **Decrypt Message** - Enter ciphertext and password to decrypt
3. **Demonstrate Honey Effect** - Interactive demo showing honey encryption
4. **Exit** - Exit the program

### Demonstration

Select option 3 to see the honey encryption in action:

1. Enter a secret message to encrypt
2. Enter a CORRECT password for encryption
3. The program encrypts the message and shows the ciphertext
4. Decrypt with the CORRECT password → returns the real message
5. Decrypt with WRONG passwords → returns believable FAKE plaintexts

The key insight: An attacker cannot distinguish between correct and incorrect password attempts because every decryption returns valid-looking output!

## Security Note

In a real-world deployment:
- The DTE mapping should be stored persistently (database, file)
- The fake plaintext generator should be trained on actual data
- Additional security measures should be implemented

## Dependencies

- OpenSSL (libssl-dev)
- C++17 compatible compiler (g++)

## License

Academic project for Cryptography and Network Security course.

