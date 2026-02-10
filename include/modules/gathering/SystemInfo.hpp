#pragma once
#include "interfaces/IGatheringMethod.hpp"

class SystemInfoMethod : public IGatheringMethod {
public:
    std::string getName() const override { return "SystemInfo"; }
    bool trigger() override;
};
