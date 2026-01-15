#pragma once
#include <string>

class IImpactMethod {
public:
    virtual ~IImpactMethod() = default;
    virtual std::string getName() const = 0;
    virtual bool trigger() = 0;
    virtual bool restore() = 0;
};
