#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

const std::string SHADOW_IP = "127.0.0.1";
const int SHADOW_PORT = 9999;
const bool USE_SSL = false;

const std::string OS_PLATFORM = "linux";
const std::string USER_AGENT = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.36";
const int DEFAULT_SLEEP_SEC = 5;
const int JITTER_PERCENT = 1;

const int HTTP_BUFFER_SIZE = 4096;

#endif