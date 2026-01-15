#pragma once
#include "interfaces/IModule.hpp"
#include "interfaces/IExfilMethod.hpp"
#include <vector>
#include <memory>

class Exfiltration : public IModule {
private:
    std::vector<std::unique_ptr<IExfilMethod>> methods;
public:
    Exfiltration();
    std::string getName() const override { return "Exfiltration"; }
    bool execute(const std::string& args = "") override;
    void restore() override;
};
