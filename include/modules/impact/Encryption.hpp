#pragma once
#include "interfaces/IImpactMethod.hpp"

class EncryptionMethod : public IImpactMethod {
public:
    std::string getName() const override { return "RunControl"; }
    bool trigger() override;
    bool restore() override;
};
