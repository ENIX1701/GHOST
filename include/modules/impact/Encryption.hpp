#pragma once
#include "interfaces/IImpactMethod.hpp"
#include "interfaces/IEncryptionStrategy.hpp"
#include <memory>

class EncryptionMethod : public IImpactMethod {
private:
    std::unique_ptr<IEncryptionStrategy> strategy;

    void applyCrypto(bool encrypt);

public:
    EncryptionMethod();
    std::string getName() const override { return "Encryption"; }
    bool trigger() override;
    bool restore() override;
};
