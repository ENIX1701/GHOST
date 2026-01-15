#pragma once
#include <string>
#include <vector>

class CryptoUtils {
public:
    static std::string XorEncrypt(const std::string& data, const std::string key);
    static std::string AesEncrypt(const std::string& data, const std::string key);
    static std::string ChaChaEncrypt(const std::string& data, const std::string key);
};
