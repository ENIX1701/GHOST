#ifndef API_SCHEMA_HPP
#define API_SCHEMA_HPP

#include <string>
#include <vector>
#include <sstream>
#include <optional>
#include "utils.hpp"

// DTOs from SHADOW
struct GhostDto {
    std::string id;
    std::string hostname;
    std::string os;

    std::string toJson() const {
        std::stringstream json;
        json << "{"
            << "\"id\":\"" << id << "\","
            << "\"hostname\":\"" << Utils::jsonEscape(hostname) << "\","
            << "\"os\":\"" << os << "\","
            << "\"last_seen\":0"
            << "}";

        return json.str();
    }
};

struct TaskResultDto {
    std::string taskId;
    std::string status;
    std::string output;

    std::string toJson() const {
        std::stringstream json;
        json << "{"
            << "\"task_id\":\"" << taskId << "\","
            << "\"status\":\"" << status << "\","
            << "\"output\":\"" << Utils::jsonEscape(output) << "\""
            << "}";

        return json.str();
    }
};

struct HeartbeatRequestDto {
    std::string id;
    std::vector<TaskResultDto> results;

    std::string toJson() const {
        std::stringstream json;
        json << "{"
            << "\"id\":\"" << id << "\",";

        if (!results.empty()) {
            json << ",\"results\":[";

            for (size_t i = 0; i < results.size(); i++) {
                json << results[i].toJson();

                if (i < results.size() - 1) {
                    json << ",";
                }
            }
        }
            
        json << "}";

        return json.str();
    }
};

struct TaskDefinitionDto {
    std::string id;
    std::string command;
    std::string args;

    // TODO: server response json parsing
};

struct HeartbeatResponseDto {
    int64_t sleepInterval;
    int64_t jitterPercent;
    std::vector<TaskDefinitionDto> tasks;
};

#endif