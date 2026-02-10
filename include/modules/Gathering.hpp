#pragma once
#include "interfaces/IModule.hpp"
#include "interfaces/IGatheringMethod.hpp"
#include <memory>
#include <vector>

class Gathering : public IModule {
private:
    std::vector<std::unique_ptr<IGatheringMethod>> methods;
public:
    Gathering();
    std::string getName() const override { return "Gathering"; }
    bool execute(const std::string& args = "") override;
    void restore() override {};
};
