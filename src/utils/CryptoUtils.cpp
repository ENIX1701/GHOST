#include "utils/CryptoUtils.hpp"

std::string CryptoUtils::XorEncrypt(const std::string& data, const std::string key) {
    std::string output = data;

    for (size_t i = 0; i < data.size(); i++) {
        output[i] = data[i] ^ key[i % key.size()];
    }

    return output;
}

std::string CryptoUtils::AesEncrypt(const std::string& data, const std::string key) {
    return data; // TODO: implement
}

std::string CryptoUtils::ChaChaEncrypt(const std::string& data, const std::string key) {
    return data; // TODO: implement
}
