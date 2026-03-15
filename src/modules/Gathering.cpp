#include "modules/Gathering.hpp"
#include "utils/Logger.hpp"

#ifdef GATHER_SYSINFO
    #include "modules/gathering/SystemInfo.hpp"
#endif
#ifdef GATHER_SSH
    #include "modules/gathering/Ssh.hpp"
#endif
#ifdef GATHER_ETCPASSWD
    #include "modules/gathering/EtcPasswd.hpp"
#endif
#ifdef GATHER_ETCSHADOW
    #include "modules/gathering/EtcShadow.hpp"
#endif

Gathering::Gathering() {
    #ifdef GATHER_SYSINFO
    methods.push_back(std::make_unique<SystemInfoMethod>());
    #endif
    #ifdef GATHER_SSH
    methods.push_back(std::make_unique<SshMethod>());
    #endif
    #ifdef GATHER_ETCPASSWD
    methods.push_back(std::make_unique<EtcPasswdMethod>());
    #endif
    #ifdef GATHER_ETCSHADOW
    methods.push_back(std::make_unique<EtcShadowMethod>());
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
