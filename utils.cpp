#include "utils.h"
#include <openssl/rand.h>
#include <stdexcept>
#include <algorithm>
#include <fstream>

namespace utils {
    
    std::string bytesToHex(const std::vector<uint8_t>& data) {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (uint8_t byte : data) {
            ss << std::setw(2) << static_cast<int>(byte);
        }
        return ss.str();
    }
    
    std::vector<uint8_t> hexToBytes(const std::string& hex) {
        if (hex.length() % 2 != 0) {
            throw std::invalid_argument("Hex string must have even length");
        }
        
        std::vector<uint8_t> data;
        data.reserve(hex.length() / 2);
        
        for (size_t i = 0; i < hex.length(); i += 2) {
            std::string byteStr = hex.substr(i, 2);
            uint8_t byte = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));
            data.push_back(byte);
        }
        
        return data;
    }
    
    std::vector<uint8_t> generateRandomBytes(size_t length) {
        std::vector<uint8_t> data(length);
        if (RAND_bytes(data.data(), length) != 1) {
            throw std::runtime_error("Failed to generate random bytes");
        }
        return data;
    }
    
    std::vector<uint8_t> stringToBytes(const std::string& str) {
        return std::vector<uint8_t>(str.begin(), str.end());
    }
    
    std::string bytesToString(const std::vector<uint8_t>& data) {
        return std::string(data.begin(), data.end());
    }
    
    // Mapping file name for storing seed-plaintext pairs
    const std::string MAPPING_FILE = "mappings.txt";
    
    bool saveMapping(const std::string& seed, const std::string& plaintext) {
        std::ofstream outFile(MAPPING_FILE, std::ios::app);
        if (!outFile.is_open()) {
            return false;
        }
        // Store as: seed|plaintext
        outFile << seed << "|" << plaintext << "\n";
        outFile.close();
        return true;
    }
    
    std::string loadMapping(const std::string& seed) {
        std::ifstream inFile(MAPPING_FILE);
        if (!inFile.is_open()) {
            return "";
        }
        
        std::string line;
        while (std::getline(inFile, line)) {
            size_t delimiterPos = line.find('|');
            if (delimiterPos != std::string::npos) {
                std::string storedSeed = line.substr(0, delimiterPos);
                std::string plaintext = line.substr(delimiterPos + 1);
                if (storedSeed == seed) {
                    inFile.close();
                    return plaintext;
                }
            }
        }
        
        inFile.close();
        return "";
    }
    
    bool seedExists(const std::string& seed) {
        return !loadMapping(seed).empty();
    }
}
