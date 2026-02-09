#pragma once
#include "interfaces/IImpactMethod.hpp"

class WiperMethod : public IImpactMethod {
public:
    std::string getName() const override { return "Wiper"; }
    bool trigger() override;
    bool restore() override;
};
