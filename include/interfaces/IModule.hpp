#pragma once
#include <string>

class IModule {
public:
    virtual ~IModule() = default;
    virtual std::string getName() const = 0;
    virtual bool execute(const std::string& args = "") = 0;
};
