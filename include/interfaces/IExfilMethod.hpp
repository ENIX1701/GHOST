#pragma once
#include <string>
#include <vector>

class IExfilMethod {
public:
    virtual ~IExfilMethod() = default;
    virtual std::string getName() const = 0;
    virtual bool canHandle(size_t sizeByte) = 0;
    virtual bool send(const std::string& file, const std::string& data) = 0;
};
