#pragma once
#include <string>

class SystemUtils {
public:
    static std::string GetUserHome();
    static std::string GetProcessPath();
    static std::string ExecuteCommand(const std::string& command);
};
