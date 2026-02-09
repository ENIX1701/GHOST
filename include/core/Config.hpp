#pragma once
#include <string>

namespace Config {
    constexpr int DEFAULT_SLEEP_SEC = 5;
    constexpr int DEFAULT_JITTER_PERCENT = 10;

    // add safeguard for now
    const std::string ENCRYPTION_KEY = ""; // yes i've accidentally encrypted my WSL

    std::string GetShadowUrl();
    std::string GetShadowPort();
    std::string GetUserAgent();
};
