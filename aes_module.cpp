#include "aes_module.h"
#include "utils.h"
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <stdexcept>
#include <cstring>

namespace aes_module {
    
    std::vector<uint8_t> deriveKey(const std::string& password) {
        // Use SHA-256 to derive a 32-byte key from password
        std::vector<uint8_t> key(32);
        SHA256(reinterpret_cast<const uint8_t*>(password.c_str()), 
               password.length(), 
               key.data());
        return key;
    }
    
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext, 
                                 const std::vector<uint8_t>& key) {
        if (key.size() != 32) {
            throw std::invalid_argument("Key must be 32 bytes for AES-256");
        }
        
        // Generate random IV (16 bytes for AES)
        std::vector<uint8_t> iv = utils::generateRandomBytes(AES_BLOCK_SIZE);
        
        // Create and initialize the encryption context
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            throw std::runtime_error("Failed to create cipher context");
        }
        
        // Initialize encryption operation
        if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, 
                               key.data(), iv.data()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to initialize encryption");
        }
        
        // Calculate buffer size (plaintext + block size for padding)
        std::vector<uint8_t> ciphertext(plaintext.size() + AES_BLOCK_SIZE);
        int outLen1 = 0;
        int outLen2 = 0;
        
        // Encrypt the plaintext
        if (EVP_EncryptUpdate(ctx, ciphertext.data(), &outLen1, 
                              plaintext.data(), plaintext.size()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Encryption failed");
        }
        
        // Finalize encryption (add padding)
        if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + outLen1, &outLen2) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Encryption finalization failed");
        }
        
        EVP_CIPHER_CTX_free(ctx);
        
        // Resize to actual ciphertext size
        ciphertext.resize(outLen1 + outLen2);
        
        // Prepend IV to ciphertext
        std::vector<uint8_t> result;
        result.reserve(iv.size() + ciphertext.size());
        result.insert(result.end(), iv.begin(), iv.end());
        result.insert(result.end(), ciphertext.begin(), ciphertext.end());
        
        return result;
    }
    
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext, 
                                  const std::vector<uint8_t>& key) {
        if (key.size() != 32) {
            throw std::invalid_argument("Key must be 32 bytes for AES-256");
        }
        
        if (ciphertext.size() < AES_BLOCK_SIZE) {
            throw std::invalid_argument("Ciphertext too short");
        }
        
        // Extract IV from beginning of ciphertext
        std::vector<uint8_t> iv(ciphertext.begin(), 
                                 ciphertext.begin() + AES_BLOCK_SIZE);
        std::vector<uint8_t> encryptedData(ciphertext.begin() + AES_BLOCK_SIZE,
                                            ciphertext.end());
        
        // Create and initialize the decryption context
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) {
            throw std::runtime_error("Failed to create cipher context");
        }
        
        // Initialize decryption operation
        if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr, 
                               key.data(), iv.data()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Failed to initialize decryption");
        }
        
        // Calculate buffer size
        std::vector<uint8_t> plaintext(encryptedData.size() + AES_BLOCK_SIZE);
        int outLen1 = 0;
        int outLen2 = 0;
        
        // Decrypt the ciphertext
        if (EVP_DecryptUpdate(ctx, plaintext.data(), &outLen1, 
                              encryptedData.data(), encryptedData.size()) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Decryption failed");
        }
        
        // Finalize decryption (remove padding)
        if (EVP_DecryptFinal_ex(ctx, plaintext.data() + outLen1, &outLen2) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            throw std::runtime_error("Decryption finalization failed");
        }
        
        EVP_CIPHER_CTX_free(ctx);
        
        // Resize to actual plaintext size
        plaintext.resize(outLen1 + outLen2);
        
        return plaintext;
    }
    
    std::string encryptSeed(const std::vector<uint8_t>& seed, 
                            const std::string& password) {
        std::vector<uint8_t> key = deriveKey(password);
        std::vector<uint8_t> ciphertext = encrypt(seed, key);
        return utils::bytesToHex(ciphertext);
    }
    
    std::vector<uint8_t> decryptSeed(const std::string& ciphertext, 
                                      const std::string& password) {
        std::vector<uint8_t> key = deriveKey(password);
        std::vector<uint8_t> ciphertextBytes = utils::hexToBytes(ciphertext);
        return decrypt(ciphertextBytes, key);
    }
    
    std::vector<uint8_t> generateSeed(const std::string& plaintext) {
        // Use SHA-256 to generate a deterministic seed from plaintext
        std::vector<uint8_t> seed(32);
        SHA256(reinterpret_cast<const uint8_t*>(plaintext.c_str()), 
               plaintext.length(), 
               seed.data());
        return seed;
    }
}
