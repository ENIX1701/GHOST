#pragma once
#include "interfaces/IGatheringMethod.hpp"

class SshMethod : public IGatheringMethod {
public:
    std::string getName() const override { return "SSH"; }
    bool trigger() override;
};
