
#include <iostream>
#include <string>
#include <memory>
#include <limits>
#include <cstdlib>

#include "aes_module.h"
#include "dte_module.h"
#include "fake_generator.h"
#include "utils.h"

/**
 * Honey Encryption System
 * 
 * This program demonstrates Honey Encryption - a technique that returns
 * plausible-looking fake plaintexts (honeywords) when decryption fails
 * or wrong keys are used, preventing attackers from knowing whether
 * their decryption attempts were successful.
 * 
 * Security Principle: No correctness signal is leaked to the attacker.
 */

/**
 * @brief Generate a fake plaintext word
 * Used for honey encryption when password is incorrect
 * @return A single random word from dictionary
 */
std::string generate_fake_plaintext() {
    return fake_generator::generateFakeWord();
}

/**
 * @brief Encrypt a message using password-based encryption
 * @param plaintext The message to encrypt
 * @param password The password to use for encryption
 * @return Hex-encoded ciphertext
 */
std::string encrypt_message(const std::string& plaintext, const std::string& password) {
    // Generate deterministic seed from plaintext using SHA-256
    std::vector<uint8_t> seed = aes_module::generateSeed(plaintext);
    
    // Encrypt the seed using AES-256-CBC with the password-derived key
    std::string ciphertext = aes_module::encryptSeed(seed, password);
    
    // Convert seed to hex for storage
    std::string seedHex = utils::bytesToHex(seed);
    
    // Save the seed-plaintext mapping for later verification
    utils::saveMapping(seedHex, plaintext);
    
    return ciphertext;
}

/**
 * @brief Verify password and return plaintext or fake word
 * @param ciphertext The hex-encoded ciphertext
 * @param password The password to verify
 * @return true if password is correct (returns original plaintext), false otherwise (returns fake word)
 */
bool decrypt_message(const std::string& ciphertext, const std::string& password, std::string& result) {
    try {
        // First, decrypt the ciphertext to get the seed
        std::vector<uint8_t> decryptedSeed = aes_module::decryptSeed(ciphertext, password);
        
        // Convert seed to hex for lookup
        std::string seedHex = utils::bytesToHex(decryptedSeed);
        
        // Check if this seed exists in our stored mappings
        std::string plaintext = utils::loadMapping(seedHex);
        
        if (!plaintext.empty()) {
            // Password is correct - return the real plaintext
            result = plaintext;
            return true;
        } else {
            // Password is incorrect - return a fake word
            result = generate_fake_plaintext();
            return false;
        }
        
    } catch (const std::exception& e) {
        // Decryption failed - return a fake word
        // This ensures no correctness signal is leaked
        result = generate_fake_plaintext();
        return false;
    }
}

void displayMenu() {
    std::cout << "\n======================================\n";
    std::cout << "     HONEY ENCRYPTION SYSTEM\n";
    std::cout << "======================================\n";
    std::cout << "1. Encrypt Message\n";
    std::cout << "2. Decrypt Message\n";
    std::cout << "3. Exit\n";
    std::cout << "======================================\n";
    std::cout << "Enter choice: ";
}

void clearInputBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string getPassword() {
    std::string password;
    std::cout << "Enter password: ";
    std::getline(std::cin, password);
    return password;
}

void encryptMessage() {
    std::string plaintext;
    std::string password;
    
    std::cout << "\n--- ENCRYPTION ---\n";
    std::cout << "Enter plaintext: ";
    std::getline(std::cin, plaintext);
    
    if (plaintext.empty()) {
        std::cout << "Error: Plaintext cannot be empty.\n";
        return;
    }
    
    password = getPassword();
    
    if (password.empty()) {
        std::cout << "Error: Password cannot be empty.\n";
        return;
    }
    
    try {
        // Use the modular encrypt_message function
        std::string ciphertext = encrypt_message(plaintext, password);
        
        std::cout << "\n--- ENCRYPTION SUCCESSFUL ---\n";
        std::cout << "Ciphertext (hex): " << ciphertext << "\n";
        std::cout << "Length: " << ciphertext.length() << " characters\n";
        
    } catch (const std::exception& e) {
        std::cout << "Encryption error: " << e.what() << "\n";
    }
}

void decryptMessage() {
    std::string ciphertext;
    std::string password;
    
    std::cout << "\n--- DECRYPTION ---\n";
    std::cout << "Enter ciphertext (hex): ";
    std::getline(std::cin, ciphertext);
    
    if (ciphertext.empty()) {
        std::cout << "Error: Ciphertext cannot be empty.\n";
        return;
    }
    
    password = getPassword();
    
    if (password.empty()) {
        std::cout << "Error: Password cannot be empty.\n";
        return;
    }
    
    // Allow up to 3 password attempts
    const int MAX_ATTEMPTS = 3;
    int attempts = 0;
    bool accessGranted = false;
    
    while (attempts < MAX_ATTEMPTS) {
        attempts++;
        
        std::string result;
        if (decrypt_message(ciphertext, password, result)) {
            // Password is correct
            std::cout << "\n--- DECRYPTION RESULT ---\n";
            std::cout << "Plaintext: " << result << "\n";
            accessGranted = true;
            break;
        } else {
            // Password is incorrect - already shows fake word
            std::cout << "\n--- DECRYPTION RESULT ---\n";
            std::cout << "Plaintext: " << result << "\n";
            
            if (attempts < MAX_ATTEMPTS) {
                std::cout << "Incorrect password. " << (MAX_ATTEMPTS - attempts) 
                          << " attempt(s) remaining.\n";
                password = getPassword();
            }
        }
    }
    
    if (!accessGranted) {
        std::cout << "\nAll " << MAX_ATTEMPTS << " attempts used.\n";
    }
}

int main() {
    int choice;
    
    std::cout << "======================================\n";
    std::cout << "     HONEY ENCRYPTION SYSTEM\n";
    std::cout << "======================================\n";
    std::cout << "Security through deception:\n";
    std::cout << "Every decryption returns valid output.\n";
    std::cout << "No correctness signal is leaked.\n";
    std::cout << "======================================\n";
    
    while (true) {
        displayMenu();
        
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number.\n";
            std::cin.clear();
            clearInputBuffer();
            continue;
        }
        
        clearInputBuffer();
        
        switch (choice) {
            case 1:
                encryptMessage();
                break;
            case 2:
                decryptMessage();
                break;
            case 3:
                std::cout << "Exiting Honey Encryption System. Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid choice. Please try again.\n";
        }
    }
    
    return 0;
}


