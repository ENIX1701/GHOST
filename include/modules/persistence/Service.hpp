#pragma once
#include "interfaces/IPersistenceMethod.hpp"

class ServiceMethod : public IPersistenceMethod {
private:
    const std::string SERVICE_NAME = "ghost.service";
public:
    std::string getName() const override { return "Systemd service"; }
    bool install() override;
    bool remove() override;
};
