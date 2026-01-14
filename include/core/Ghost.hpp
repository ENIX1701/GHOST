#pragma once
#include <map>
#include <string>
#include <memory>
#include "interfaces/IModule.hpp"

class Ghost {
private:
    std::map<std::string, std::unique_ptr<IModule>> modules;
public:
    Ghost();
    void run();
};
