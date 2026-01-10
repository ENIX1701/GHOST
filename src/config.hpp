#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

const std::string SHADOW_IP = "127.0.0.1";
const int SHADOW_PORT = 9999;
const bool USE_SSL = false;

const std::string OS_PLATFORM = "linux";
const int DEFAULT_SLEEP_SEC = 60;
const int JITTER_PERCENT = 20;

const int HTTP_BUFFER_SIZE = 4096;

#endif