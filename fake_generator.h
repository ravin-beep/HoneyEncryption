#ifndef FAKE_GENERATOR_H
#define FAKE_GENERATOR_H

#include <string>
#include <vector>
#include <random>

namespace fake_generator {
    
    /**
     * @brief Generate a fake plaintext using one of two strategies:
     * 1. Frequency-Based Model (credential format)
     * 2. Markov Chain Generator (trained on corpus)
     * 
     * @return A realistic-looking fake plaintext
     */
    std::string generateFakePlaintext();
    
    /**
     * @brief Generate fake credentials in format:
     * USERNAME: <random_name>
     * PASSWORD: <random_string>
     * EMAIL: <random_email>
     * 
     * @return Fake credential string
     */
    std::string generateFakeCredentials();
    
    /**
     * @brief Generate a single realistic-looking fake word
     * Used for honey encryption when password is incorrect
     * 
     * @return A single random word from dictionary
     */
    std::string generateFakeWord();
    
    /**
     * @brief Generate text using simple Markov chain
     * @param length Approximate length of generated text
     * @return Markov-generated text
     */
    std::string generateMarkovText(size_t length);
    
    /**
     * @brief Generate a random name
     * @return Random name string
     */
    std::string generateRandomName();
    
    /**
     * @brief Generate a random password-like string
     * @param length Length of password
     * @return Random password string
     */
    std::string generateRandomPassword(size_t length);
    
    /**
     * @brief Generate a random email
     * @return Random email string
     */
    std::string generateRandomEmail();
}

#endif // FAKE_GENERATOR_H
