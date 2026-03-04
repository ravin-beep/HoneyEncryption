cmake_minimum_required(VERSION 3.10)
project(HoneyEncryption)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenSSL
find_package(OpenSSL REQUIRED)

# Include directories
include_directories(${OPENSSL_INCLUDE_DIR})

# Add executable
add_executable(honey_encrypt 
    main.cpp 
    aes_module.cpp 
    dte_module.cpp 
    fake_generator.cpp 
    utils.cpp
)

# Link OpenSSL
target_link_libraries(honey_encrypt ${OPENSSL_LIBRARIES})

# Add compile options
target_compile_options(honey_encrypt PRIVATE -Wall -Wextra)
