#pragma once
#include "interface/IModule.hpp"
#include "interface/IImpactMethod.hpp"
#include <vector>
#include <memory>

class Impact : public IModule {
private:
    std::vector<std::unique_ptr<IImpactMethod>> methods;
public:
    Impact();
    std::string getName() const override { return "Impact"; }
    bool execute(const std::string& arg = "") override;
    void restore();
};
