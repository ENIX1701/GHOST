#include "modules/impact/Encryption.hpp"
#include "utils/Logger.hpp"
#include "utils/CryptoUtils.hpp"
#include "utils/FileUtils.hpp"

void EncryptionMethod::trigger() {
    auto file = FileUtils::ListFilesRecursively(SystemUtils::GetUserHome());

    for (const auto& file : files) {
        LOG_INFO("Encrypting {}", file)
    }
}

void EncryptionMethod::restore() {
    
}
