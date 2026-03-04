#include "dte_module.h"
#include "fake_generator.h"
#include <openssl/sha.h>
#include <stdexcept>

namespace dte_module {
    
    // Internal structure to store plaintext-seed mappings
    struct DTEMapping {
        std::map<std::vector<uint8_t>, std::string> seedToPlaintext;
        std::map<std::string, std::vector<uint8_t>> plaintextToSeed;
    };
    
    DTE::DTE() : mapping_(std::make_shared<DTEMapping>()) {}
    
    DTE::~DTE() = default;
    
    std::vector<uint8_t> DTE::generateSeed(const std::string& plaintext) const {
        // Use SHA-256 to generate a fixed-size seed from plaintext
        std::vector<uint8_t> seed(32);
        SHA256(reinterpret_cast<const uint8_t*>(plaintext.c_str()), 
               plaintext.length(), 
               seed.data());
        return seed;
    }
    
    std::vector<uint8_t> DTE::encode(const std::string& plaintext) {
        // Generate seed from plaintext
        std::vector<uint8_t> seed = generateSeed(plaintext);
        
        // Store the mapping
        storeMapping(plaintext);
        
        return seed;
    }
    
    std::string DTE::decode(const std::vector<uint8_t>& seed) {
        // Check if this is a legitimate seed
        auto it = mapping_->seedToPlaintext.find(seed);
        
        if (it != mapping_->seedToPlaintext.end()) {
            // Legitimate plaintext found
            return it->second;
        }
        
        // Seed is not legitimate - generate fake plaintext
        // The fake generator creates realistic-looking but fake credentials
        return fake_generator::generateFakePlaintext();
    }
    
    void DTE::storeMapping(const std::string& plaintext) {
        std::vector<uint8_t> seed = generateSeed(plaintext);
        
        // Store both mappings for bidirectional lookup
        mapping_->seedToPlaintext[seed] = plaintext;
        mapping_->plaintextToSeed[plaintext] = seed;
    }
    
    bool DTE::isLegitimate(const std::vector<uint8_t>& seed) const {
        return mapping_->seedToPlaintext.find(seed) != 
               mapping_->seedToPlaintext.end();
    }
}
