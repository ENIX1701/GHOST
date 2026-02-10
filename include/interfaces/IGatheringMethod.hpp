#pragma once
#include <string>

class IGatheringMethod {
public:
    virtual ~IGatheringMethod() = default;
    virtual std::string getName() const = 0;
    virtual bool trigger() = 0;
};
