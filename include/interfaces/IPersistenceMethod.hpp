#pragma once
#include <string>

class IPersistenceMethod {
public:
    virtual ~IPersistenceMethod() = default;
    virtual std::string getName() const = 0;
    virtual bool install() = 0;
    virtual bool remove() = 0;
};
