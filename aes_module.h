#ifndef AES_MODULE_H
#define AES_MODULE_H

#include <vector>
#include <string>
#include <cstdint>

// AES block size in bytes (128 bits)
#define AES_BLOCK_SIZE 16

namespace aes_module {
    /**
     * @brief Derive AES key from password using SHA-256
     * @param password User password
     * @return 32-byte AES key
     */
    std::vector<uint8_t> deriveKey(const std::string& password);
    
    /**
     * @brief Encrypt data using AES-256-CBC
     * @param plaintext Data to encrypt
     * @param key 32-byte AES key
     * @return Ciphertext (IV + ciphertext)
     */
    std::vector<uint8_t> encrypt(const std::vector<uint8_t>& plaintext, 
                                 const std::vector<uint8_t>& key);
    
    /**
     * @brief Decrypt data using AES-256-CBC
     * @param ciphertext Data to decrypt (IV + ciphertext)
     * @param key 32-byte AES key
     * @return Decrypted plaintext
     */
    std::vector<uint8_t> decrypt(const std::vector<uint8_t>& ciphertext, 
                                  const std::vector<uint8_t>& key);
    
    /**
     * @brief Encrypt seed (fixed-size) using AES-256-CBC
     * @param seed 16 or 32-byte seed to encrypt
     * @param password User password
     * @return Hex-encoded ciphertext
     */
    std::string encryptSeed(const std::vector<uint8_t>& seed, 
                            const std::string& password);
    
    /**
     * @brief Decrypt seed using AES-256-CBC
     * @param ciphertext Hex-encoded ciphertext
     * @param password User password
     * @return Decrypted seed
     */
    std::vector<uint8_t> decryptSeed(const std::string& ciphertext, 
                                      const std::string& password);
    
    /**
     * @brief Generate deterministic seed from plaintext using SHA-256
     * @param plaintext The plaintext message
     * @return 32-byte deterministic seed
     */
    std::vector<uint8_t> generateSeed(const std::string& plaintext);
}

#endif // AES_MODULE_H
