#pragma once
#include "interfaces/IPersistenceMethod.hpp"
#include <vector>

class RunControlMethod : public IPersistenceMethod {
private:
    // pair<FilePath, IsEstablished>
    std::vector<std::pair<std::string, bool>> targets;
    std::string payload;
public:
    RunControlMethod();
    std::string getName() const override { return "RunControl"; }
    bool install() override;
    bool remove() override;
};
