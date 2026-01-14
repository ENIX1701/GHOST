#pragma once
#include "interfaces/IModule.hpp"
#include "interfaces/IPersistenceMethod.hpp"
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <memory>

class Persistence : public IModule {
private:
    std::vector<std::unique_ptr<IPersistenceMethod>> methods;
public:
    Persistence();
    std::string getName() const override { return "Persistence"; }
    bool execute(const std::string& args = "") override;
};
