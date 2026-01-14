#pragma once
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <array>

class SystemUtils {
public:
    static std::string GetUserHome() {
        const char* home = std::getenv("HOME");

        return home ? std::string(home) : "/tmp";
    }

    static std::string GetProcessPath() {
        char buffer[1024];
        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);

        if (len != -1) {
            buffer[len] = '\0';
            return std::string(buffer);
        }

        return "";
    }

    static std::string ExecuteCommand(const std::string& command) {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);

        if (!pipe) return "";

        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
            result += buffer.data();
        }

        return result;
    }
};
