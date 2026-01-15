#pragma once
#include "interfaces/IModule.hpp"
#include "interfaces/IImpactMethod.hpp"
#include <vector>
#include <memory>

class Impact : public IModule {
private:
    std::vector<std::unique_ptr<IImpactMethod>> methods;
public:
    Impact();
    std::string getName() const override { return "Impact"; }
    bool execute(const std::string& args = "") override;
    void restore() override;
};
