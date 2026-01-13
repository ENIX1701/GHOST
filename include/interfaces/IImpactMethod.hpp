#pragma once
#include <string>

class IImpactMethod {
public:
    virtual ~IImpactMethod() = default;
    virtual std::string getName() const = 0;
    virtual void trigger() = 0;
    virtual void restore() = 0;
};
