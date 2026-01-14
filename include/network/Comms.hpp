#pragma once
#include <string>
#include <cpr/cpr.h>

class Comms {
public:
    static std::string SendPost(const std::string& endpoint, const std::string& payload);
    static bool UploadFile(const std::string& endpoint, const std::string& filename, const std::string& fileContent);
};
