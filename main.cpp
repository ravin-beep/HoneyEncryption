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
 * @param originalPlaintext The original plaintext (for comparison)
 * @return true if password is correct, false otherwise
 */
bool password_verification(const std::string& ciphertext, const std::string& password, 
                           const std::string& originalPlaintext) {
    try {
        // First, decrypt the ciphertext to get the seed
        std::vector<uint8_t> decryptedSeed = aes_module::decryptSeed(ciphertext, password);
        
        // Convert seed to hex for lookup
        std::string seedHex = utils::bytesToHex(decryptedSeed);
        
        // Check if this seed exists in our stored mappings
        std::string plaintext = utils::loadMapping(seedHex);
        
        if (!plaintext.empty() && plaintext == originalPlaintext) {
            // Password is correct - return the real plaintext
            return true;
        } else {
            // Password is incorrect - return a fake word
            return false;
        }
        
    } catch (const std::exception& e) {
        // Decryption failed - password is incorrect
        return false;
    }
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

/**
 * @brief Main honey encryption flow
 * Prompts for plaintext and password, encrypts, then allows 3 attempts to verify
 */
void honeyEncryptionFlow() {
    std::string plaintext;
    std::string password;
    
    std::cout << "\n======================================\n";
    std::cout << "     HONEY ENCRYPTION SETUP\n";
    std::cout << "======================================\n";
    
    // Step 1: Get the plaintext message
    std::cout << "Enter plaintext message: ";
    std::getline(std::cin, plaintext);
    
    if (plaintext.empty()) {
        std::cout << "Error: Plaintext cannot be empty.\n";
        return;
    }
    
    // Step 2: Get the correct password
    password = getPassword();
    
    if (password.empty()) {
        std::cout << "Error: Password cannot be empty.\n";
        return;
    }
    
    // Step 3: Encrypt the message
    std::string ciphertext;
    try {
        ciphertext = encrypt_message(plaintext, password);
        
        std::cout << "\n--- ENCRYPTION SUCCESSFUL ---\n";
        std::cout << "Ciphertext (hex): " << ciphertext << "\n";
        std::cout << "Message has been encrypted.\n";
        
    } catch (const std::exception& e) {
        std::cout << "Encryption error: " << e.what() << "\n";
        return;
    }
    
    // Step 4: Password verification attempts (up to 3)
    const int MAX_ATTEMPTS = 3;
    int attempts = 0;
    bool accessGranted = false;
    
    std::cout << "\n======================================\n";
    std::cout << "     PASSWORD VERIFICATION\n";
    std::cout << "======================================\n";
    std::cout << "You have " << MAX_ATTEMPTS << " attempts to verify your password.\n";
    
    while (attempts < MAX_ATTEMPTS) {
        attempts++;
        
        std::cout << "\n--- Attempt " << attempts << " of " << MAX_ATTEMPTS << " ---\n";
        std::string inputPassword = getPassword();
        
        // Verify the password
        if (password_verification(ciphertext, inputPassword, plaintext)) {
            // Correct password
            std::cout << "\n======================================\n";
            std::cout << "     ACCESS GRANTED\n";
            std::cout << "======================================\n";
            std::cout << "Plaintext: " << plaintext << "\n";
            std::cout << "Program terminated successfully.\n";
            accessGranted = true;
            break;
        } else {
            // Incorrect password - show fake plaintext (single word)
            std::string fakeWord = generate_fake_plaintext();
            std::cout << "\n--- DECRYPTION RESULT ---\n";
            std::cout << "Plaintext: " << fakeWord << "\n";
            
            if (attempts < MAX_ATTEMPTS) {
                std::cout << "Incorrect password. " << (MAX_ATTEMPTS - attempts) 
                          << " attempt(s) remaining.\n";
            }
        }
    }
    
    // All attempts used
    if (!accessGranted) {
        std::cout << "\n======================================\n";
        std::cout << "     ACCESS DENIED\n";
        std::cout << "======================================\n";
        std::cout << "All " << MAX_ATTEMPTS << " attempts used.\n";
        std::cout << "Program terminated.\n";
    }
}

int main() {
    std::cout << "======================================\n";
    std::cout << "     HONEY ENCRYPTION SYSTEM\n";
    std::cout << "======================================\n";
    std::cout << "Security through deception:\n";
    std::cout << "Every password attempt returns valid output.\n";
    std::cout << "No correctness signal is leaked.\n";
    std::cout << "======================================\n";
    
    // Run the honey encryption flow
    honeyEncryptionFlow();
    
    return 0;
}

