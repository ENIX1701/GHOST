#pragma once
#include <string>

namespace Config {
    const std::string SHADOW_URL = "http://127.0.0.1:9999";
    const std::string USER_AGENT = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36";

    constexpr int DEFAULT_SLEEP_SEC = 5;
    constexpr int DEFAULT_JITTER_PERCENT = 10;

    const std::string ENCRYPTION_KEY = "GHOST_ENCRYPTION_KEY_EXAMPLE";
};
