#ifndef UTILS_H
#define UTILS_H

#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdint>

namespace utils {
    /**
     * @brief Convert bytes to hexadecimal string
     * @param data Byte vector to convert
     * @return Hex string representation
     */
    std::string bytesToHex(const std::vector<uint8_t>& data);
    
    /**
     * @brief Convert hex string to bytes
     * @param hex Hex string to convert
     * @return Byte vector
     */
    std::vector<uint8_t> hexToBytes(const std::string& hex);
    
    /**
     * @brief Generate random bytes
     * @param length Number of bytes to generate
     * @return Random byte vector
     */
    std::vector<uint8_t> generateRandomBytes(size_t length);
    
    /**
     * @brief Convert string to bytes
     * @param str String to convert
     * @return Byte vector
     */
    std::vector<uint8_t> stringToBytes(const std::string& str);
    
    /**
     * @brief Convert bytes to string
     * @param data Byte vector to convert
     * @return String
     */
    std::string bytesToString(const std::vector<uint8_t>& data);
    
    /**
     * @brief Save seed-plaintext mapping to file
     * @param seed The seed (hex string)
     * @param plaintext The plaintext
     * @return true if successful
     */
    bool saveMapping(const std::string& seed, const std::string& plaintext);
    
    /**
     * @brief Load plaintext for a given seed from file
     * @param seed The seed (hex string)
     * @return The plaintext if found, empty string otherwise
     */
    std::string loadMapping(const std::string& seed);
    
    /**
     * @brief Check if a seed exists in storage
     * @param seed The seed (hex string)
     * @return true if seed exists in storage
     */
    bool seedExists(const std::string& seed);
}

#endif // UTILS_H
