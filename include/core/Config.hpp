#pragma once
#include <string>

enum class ImpactLevel {
    Test,
    User,
    System
};

namespace Config {
    constexpr int DEFAULT_SLEEP_SEC = 5;
    constexpr int DEFAULT_JITTER_PERCENT = 10;

    // add safeguard for now
    const std::string ENCRYPTION_KEY = ""; // yes i've accidentally encrypted my WSL

    std::string GetShadowUrl();
    std::string GetShadowPort();
    ImpactLevel GetImpactLevel();
    std::string GetUserAgent();
};
