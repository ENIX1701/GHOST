#include "core/Config.hpp"
#include <cstdlib>

namespace Config {
    std::string GetShadowUrl() {
        const char* envUrl = std::getenv("SHADOW_URL");
        if (envUrl && std::string(envUrl).length() > 0) {
            return std::string(envUrl);
        }

        return SHADOW_URL;
    }

    std::string GetShadowPort() {
        const char* envPort = std::getenv("SHADOW_PORT");
        if (envPort && std::string(envPort).length() > 0) {
            return std::string(envPort);
        }

        return SHADOW_PORT;
    }
}
