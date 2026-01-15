#pragma once
#include "interfaces/IExfilMethod.hpp"

class HttpPostMethod : public IExfilMethod {
public:
    std::string getName() const override { return "HttpPost"; }
    bool canHandle(size_t sizeBytes) override { return true; }
    bool send(const std::string& filename, const std::string& data) override;
};
