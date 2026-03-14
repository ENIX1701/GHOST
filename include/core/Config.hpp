#pragma once
#include <string>

namespace Config {
    #ifndef GHOST_SLEEP_SEC
    constexpr int DEFAULT_SLEEP_SEC = 5;
    #else
    constexpr int DEFAULT_SLEEP_SEC = GHOST_SLEEP_SEC;
    #endif

    #ifndef GHOST_JITTER_PERCENT
    constexpr int DEFAULT_JITTER_PERCENT = 10;
    #else
    constexpr int DEFAULT_JITTER_PERCENT = GHOST_JITTER_PERCENT;
    #endif

    // add safeguard for now
    const std::string ENCRYPTION_KEY = ""; // yes i've accidentally encrypted my WSL
    // TODO: parametrize encryption key

    void SetGhostId(const std::string& id);
    std::string GetGhostId();

    std::string GetShadowUrl();
    std::string GetShadowPort();
    std::string GetUserAgent();
};
