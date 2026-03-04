#ifndef DTE_MODULE_H
#define DTE_MODULE_H

#include <vector>
#include <string>
#include <map>
#include <memory>

namespace dte_module {
    
    // Forward declaration
    class DTEMapping;
    using DTEMappingPtr = std::shared_ptr<DTEMapping>;
    
    /**
     * @brief Distribution Transforming Encoder (DTE)
     * 
     * The DTE encodes plaintext messages into fixed-size seeds and decodes
     * seeds back to plaintext. If a seed doesn't correspond to a legitimate
     * message, it returns a fake plaintext instead.
     */
    class DTE {
    public:
        DTE();
        ~DTE();
        
        /**
         * @brief Encode plaintext into a fixed-size seed
         * @param plaintext The plaintext to encode
         * @return 32-byte seed
         */
        std::vector<uint8_t> encode(const std::string& plaintext);
        
        /**
         * @brief Decode seed to plaintext
         * @param seed The seed to decode
         * @return Original plaintext if seed is legitimate, otherwise fake plaintext
         */
        std::string decode(const std::vector<uint8_t>& seed);
        
        /**
         * @brief Store a legitimate plaintext-seed mapping
         * @param plaintext The plaintext to store
         */
        void storeMapping(const std::string& plaintext);
        
        /**
         * @brief Check if a seed corresponds to a legitimate message
         * @param seed The seed to check
         * @return true if seed is legitimate
         */
        bool isLegitimate(const std::vector<uint8_t>& seed) const;
        
    private:
        DTEMappingPtr mapping_;
        
        /**
         * @brief Generate seed from plaintext using SHA-256
         * @param plaintext The plaintext
         * @return 32-byte seed
         */
        std::vector<uint8_t> generateSeed(const std::string& plaintext) const;
    };
    
    /**
     * @brief Shared pointer to DTE
     */
    using DTEPtr = std::shared_ptr<DTE>;
}

#endif // DTE_MODULE_H
