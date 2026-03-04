#include "fake_generator.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <random>
#include <map>
#include <algorithm>
#include <ctime>

namespace fake_generator {
    
    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Character sets for password generation
    const std::string lowercase = "abcdefghijklmnopqrstuvwxyz";
    const std::string uppercase = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string digits = "0123456789";
    const std::string special = "!@#$%^&*";
    
    // Sample names for random generation
    const std::vector<std::string> firstNames = {
        "James", "John", "Robert", "Michael", "William", 
        "David", "Richard", "Joseph", "Thomas", "Charles",
        "Mary", "Patricia", "Jennifer", "Linda", "Barbara",
        "Elizabeth", "Susan", "Jessica", "Sarah", "Karen"
    };
    
    const std::vector<std::string> lastNames = {
        "Smith", "Johnson", "Williams", "Brown", "Jones",
        "Garcia", "Miller", "Davis", "Rodriguez", "Martinez",
        "Hernandez", "Lopez", "Gonzalez", "Wilson", "Anderson"
    };
    
    // Email domains
    const std::vector<std::string> emailDomains = {
        "gmail.com", "yahoo.com", "outlook.com", 
        "hotmail.com", "mail.com", "example.com"
    };
    
    // Dictionary of realistic single words for honey encryption
    const std::vector<std::string> wordDictionary = {
        "account", "admin", "balance", "bank", "billing", "budget", "card",
        "cash", "client", "cloud", "code", "config", "credit", "crypto",
        "customer", "data", "database", "domain", "download", "driver",
        "email", "encryption", "file", "folder", "gateway", "hardware",
        "host", "image", "invoice", "key", "license", "login", "mail",
        "network", "note", "order", "packet", "password", "payment", "phone",
        "portal", "private", "profile", "project", "proxy", "public",
        "query", "record", "report", "request", "resource", "router",
        "script", "secure", "server", "service", "session", "share",
        "software", "source", "storage", "stream", "system", "token",
        "traffic", "transaction", "upload", "user", "video", "voice",
        "wallet", "web", "wireless", "worker", "zone"
    };
    
    // Markov chain training corpus
    const std::string corpus = 
        "The quick brown fox jumps over the lazy dog. "
        "Security is paramount in modern computing systems. "
        "Data protection requires strong encryption methods. "
        "Network security prevents unauthorized access. "
        "Authentication verifies user identity. "
        "Passwords should be strong and unique. "
        "Firewalls protect against malicious attacks. "
        "Encryption ensures data confidentiality. "
        "Digital signatures provide authenticity. "
        "Intrusion detection systems monitor network traffic. "
        "Malware can compromise system integrity. "
        "Backup strategies prevent data loss. "
        "Access control limits system resources. "
        "VPN creates secure remote connections. "
        "Cyber security protects digital assets.";
    
    // Markov chain model
    std::map<char, std::vector<char>> markovModel;
    
    /**
     * @brief Build Markov chain model from corpus
     */
    void buildMarkovModel() {
        if (!markovModel.empty()) return;
        
        for (size_t i = 0; i < corpus.length() - 1; ++i) {
            char c = corpus[i];
            char next = corpus[i + 1];
            if (c != ' ') {
                markovModel[c].push_back(next);
            }
        }
    }
    
    std::string generateRandomName() {
        std::uniform_int_distribution<> firstDist(0, firstNames.size() - 1);
        std::uniform_int_distribution<> lastDist(0, lastNames.size() - 1);
        
        return firstNames[firstDist(gen)] + " " + lastNames[lastDist(gen)];
    }
    
    std::string generateRandomPassword(size_t length) {
        std::string allChars = lowercase + uppercase + digits + special;
        std::uniform_int_distribution<> dist(0, allChars.length() - 1);
        
        std::string password;
        password.reserve(length);
        
        // Ensure at least one of each type
        password += lowercase[std::uniform_int_distribution<>(0, lowercase.length() - 1)(gen)];
        password += uppercase[std::uniform_int_distribution<>(0, uppercase.length() - 1)(gen)];
        password += digits[std::uniform_int_distribution<>(0, digits.length() - 1)(gen)];
        password += special[std::uniform_int_distribution<>(0, special.length() - 1)(gen)];
        
        // Fill the rest randomly
        for (size_t i = 4; i < length; ++i) {
            password += allChars[dist(gen)];
        }
        
        // Shuffle the password
        std::shuffle(password.begin(), password.end(), gen);
        
        return password;
    }
    
    std::string generateRandomEmail() {
        std::uniform_int_distribution<> firstDist(0, firstNames.size() - 1);
        std::uniform_int_distribution<> lastDist(0, lastNames.size() - 1);
        std::uniform_int_distribution<> domainDist(0, emailDomains.size() - 1);
        std::uniform_int_distribution<> numDist(100, 9999);
        
        std::string email(1, lowercase[std::uniform_int_distribution<>(0, lowercase.length() - 1)(gen)]);
        email += lastNames[lastDist(gen)];
        email += std::to_string(numDist(gen));
        email += "@";
        email += emailDomains[domainDist(gen)];
        
        return email;
    }
    
    std::string generateFakeCredentials() {
        std::string name = generateRandomName();
        std::string password = generateRandomPassword(12);
        std::string email = generateRandomEmail();
        
        std::ostringstream oss;
        oss << "USERNAME: " << name << "\n";
        oss << "PASSWORD: " << password << "\n";
        oss << "EMAIL: " << email;
        
        return oss.str();
    }
    
    std::string generateMarkovText(size_t length) {
        buildMarkovModel();
        
        if (markovModel.empty()) {
            return "Sample text generated by Markov chain model.";
        }
        
        std::string result;
        result.reserve(length);
        
        // Start with a random letter
        std::uniform_int_distribution<> letterDist(0, 25);
        char current = 'a' + letterDist(gen);
        result += current;
        
        while (result.length() < length) {
            auto it = markovModel.find(current);
            if (it == markovModel.end() || it->second.empty()) {
                // If no continuation, pick a random letter
                current = 'a' + letterDist(gen);
            } else {
                // Pick next character from model
                std::uniform_int_distribution<> 
                    nextDist(0, it->second.size() - 1);
                current = it->second[nextDist(gen)];
            }
            
            result += current;
            
            // Add spaces occasionally
            if (current == '.' || current == ' ' || 
                std::uniform_int_distribution<>(0, 20)(gen) == 0) {
                result += ' ';
            }
        }
        
        // Make it look like sentences
        if (result.length() > 10) {
            result[0] = std::toupper(result[0]);
            // Add proper sentence ending
            if (result[result.length() - 1] != '.') {
                result += ".";
            }
        }
        
        return result;
    }
    
    std::string generateFakePlaintext() {
        // Randomly choose between the two methods
        std::uniform_int_distribution<> choiceDist(0, 1);
        int choice = choiceDist(gen);
        
        if (choice == 0) {
            // Frequency-based model (credential format)
            return generateFakeCredentials();
        } else {
            // Markov chain generator
            // Generate text of similar length to typical credentials
            std::uniform_int_distribution<> lengthDist(40, 80);
            return generateMarkovText(lengthDist(gen));
        }
    }
    
    std::string generateFakeWord() {
        // Randomly select a word from the dictionary
        std::uniform_int_distribution<> dist(0, wordDictionary.size() - 1);
        return wordDictionary[dist(gen)];
    }
}
