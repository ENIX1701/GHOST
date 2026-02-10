#include "modules/Gathering.hpp"
#include "utils/Logger.hpp"

#ifdef METHOD_SYSINFO
    #include "modules/gathering/SystemInfo.hpp"
#endif

Gathering::Gathering() {
    #ifdef METHOD_SYSINFO
    methods.push_back(std::make_unique<SystemInfoMethod>());
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
