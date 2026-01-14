#include "modules/impact/Encryption.hpp"
#include "utils/Logger.hpp"
#include "utils/CryptoUtils.hpp"
#include "utils/FileUtils.hpp"
#include "utils/SystemUtils.hpp"

bool EncryptionMethod::trigger() {
    auto files = FileUtils::ListFilesRecursively(SystemUtils::GetUserHome());

    for (const auto& file : files) {
        LOG_INFO("Encrypting {}", file)
    }

    return true;
}

bool EncryptionMethod::restore() {
    return true;
}
