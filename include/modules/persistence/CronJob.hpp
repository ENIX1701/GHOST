#pragma once
#include "interfaces/IPersistenceMethod.hpp"

class CronJobMethod : public IPersistenceMethod {
public:
    std::string getName() const override { return "CronJob"; }
    bool install() override;
    bool remove() override;
};
