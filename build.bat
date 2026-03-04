@echo off
REM Honey Encryption - Build Script for Windows
REM 
REM Prerequisites:
REM 1. Install OpenSSL (https://slproweb.com/products/Win32OpenSSL.html)
REM 2. Install MinGW-w64 or Visual Studio with C++ support
REM 3. Add MinGW/bin or VS paths to PATH

echo ================================
echo Honey Encryption - Build Script
echo ================================

REM Check for OpenSSL
where openssl >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: OpenSSL not found. Please install OpenSSL development libraries.
    echo Download from: https://slproweb.com/products/Win32OpenSSL.html
    exit /b 1
)

REM Try g++ (MinGW)
where g++ >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo Found MinGW g++, compiling...
    g++ -o honey_encrypt.exe ^
        -I. ^
        main.cpp aes_module.cpp dte_module.cpp fake_generator.cpp utils.cpp ^
        -lssl -lcrypto -lws2_32
    if %ERRORLEVEL% EQU 0 (
        echo Build successful! Run: honey_encrypt.exe
    )
    exit /b %ERRORLEVEL%
)

REM Try cl (Visual Studio)
where cl >nul 2>&1
if %ERRORLEVEL% EQU 0 (
    echo Found Visual Studio cl, compiling...
    cl /EHsc /I. /DOPENSSL_USE_APPLINK ^
        main.cpp aes_module.cpp dte_module.cpp fake_generator.cpp utils.cpp ^
        /link /LIBPATH:"C:\OpenSSL-Win64\lib" libssl.lib libcrypto.lib
    if %ERRORLEVEL% EQU 0 (
        echo Build successful! Run: honey_encrypt.exe
    )
    exit /b %ERRORLEVEL%
)

echo ERROR: No C++ compiler found. Please install MinGW-w64 or Visual Studio.
exit /b 1
