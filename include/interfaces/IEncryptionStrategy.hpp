#pragma once
#include <string>

class IEncryptionStrategy {
public:
    virtual ~IEncryptionStrategy() = default;
    virtual std::string encrypt(const std::string& data, const std::string& key) = 0;
    virtual std::string decrypt(const std::string& data, const std::string& key) = 0;
};
