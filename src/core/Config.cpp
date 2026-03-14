#include "core/Config.hpp"
#include "utils/Obfuscation.hpp"
#include <cstdlib>

namespace Config {
    static std::string ghostId = "";

    void SetGhostId(const std::string& id) {
        ghostId = id;
    }

    std::string GetGhostId() {
        return ghostId;
    }

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

    std::string GetUserAgent() {
        return OBFL("Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36");
    }
}
