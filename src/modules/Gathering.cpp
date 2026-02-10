#include "modules/Gathering.hpp"
#include "utils/Logger.hpp"

#ifdef METHOD_SYSINFO
    #include "modules/gathering/SystemInfo.hpp"
#endif
#ifdef METHOD_SSH
    #include "modules/gathering/Ssh.hpp"
#endif
#ifdef METHOD_ETCPASSWD
    #include "modules/gathering/EtcPasswd.hpp"
#endif
#ifdef METHOD_ETCSHADOW
    #include "modules/gathering/EtcShadow.hpp"
#endif

Gathering::Gathering() {
    #ifdef METHOD_SYSINFO
    methods.push_back(std::make_unique<SystemInfoMethod>());
    #endif
    #ifdef METHOD_SSH
    methods.push_back(std::make_unique<Ssh>());
    #endif
    #ifdef METHOD_ETCPASSWD
    methods.push_back(std::make_unique<EtcPasswd>());
    #endif
    #ifdef METHOD_ETCSHADOW
    methods.push_back(std::make_unique<EtcShadow>());
    #endif
}

bool Gathering::execute(const std::string& args) {
    LOG_INFO("Starting information gathering")

    short successCounter = 0;
    for (auto& method : this->methods) {
        LOG_INFO("[GATHERING] Triggering method {}", method->getName())

        if (method->trigger()) {
            LOG_SUCCESS("Data gathered via {}", method->getName())
            successCounter++;
        } else {
            LOG_ERROR("{} failed", method->getName())
        }
    }

    return successCounter;
}
