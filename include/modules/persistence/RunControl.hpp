#pragma once
#include "interfaces/IPersistenceMethod.hpp"

class RunControlMethod : public IPersistenceMethod {
public:
    std::string getName() const override { return "RunControl"; }
    bool install() override;
    bool remove() override;
};
