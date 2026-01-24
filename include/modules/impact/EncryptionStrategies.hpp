#pragma once
#include "interfaces/IEncryptionStrategy.hpp"
#include "utils/CryptoUtils.hpp"

class XorEncryptionStrategy : public IEncryptionStrategy {
public:
    std::string encrypt(const std::string& data, const std::string& key) override {
        return CryptoUtils::XorEncrypt(data, key);
    }
    
    std::string decrypt(const std::string& data, const std::string& key) override {
        return CryptoUtils::XorEncrypt(data, key);  // symmetric
    }
};

class AesEncryptionStrategy : public IEncryptionStrategy {
public:
    std::string encrypt(const std::string& data, const std::string& key) override {
        return CryptoUtils::AesEncrypt(data, key);
    }
    
    std::string decrypt(const std::string& data, const std::string& key) override {
        return CryptoUtils::AesEncrypt(data, key);  // symmetric
    }
};

class ChaChaEncryptionStrategy : public IEncryptionStrategy {
public:
    std::string encrypt(const std::string& data, const std::string& key) override {
        return CryptoUtils::ChaChaEncrypt(data, key);
    }
    
    std::string decrypt(const std::string& data, const std::string& key) override {
        return CryptoUtils::ChaChaEncrypt(data, key);  // symmetric
    }
};
