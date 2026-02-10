#pragma once
#include "interfaces/IGatheringMethod.hpp"

class EtcPasswdMethod : public IGatheringMethod {
public:
    std::string getName() const override { return "/etc/passwd"; }
    bool trigger() override;
};
