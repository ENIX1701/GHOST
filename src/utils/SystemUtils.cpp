#include "utils/SystemUtils.hpp"
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <array>
#include <memory>

std::string SystemUtils::GetUserHome() {
    static std::string cachedHome;
    if (!cachedHome.empty()) return cachedHome;

    const char* home = std::getenv("HOME");
    cachedHome = home ? std::string(home) : "/tmp";

    return cachedHome;
}

std::string SystemUtils::GetProcessPath() {
    char buffer[1024];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);

    if (len != -1) {
        buffer[len] = '\0';
        return std::string(buffer);
    }

    return "";
}

std::string SystemUtils::ExecuteCommand(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);

    if (!pipe) return "";

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}
