#pragma once
#include "interfaces/IGatheringMethod.hpp"

class EtcShadowMethod : public IGatheringMethod {
public:
    std::string getName() const override { return "/etc/shadow"; }
    bool trigger() override;
};
