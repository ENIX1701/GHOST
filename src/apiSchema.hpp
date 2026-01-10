#ifndef API_SCHEMA_HPP
#define API_SCHEMA_HPP

#include <string>
#include <vector>
#include <sstream>
#include <optional>
#include "utils.hpp"

// === REQUEST DTOs ===
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
            << "\"id\":\"" << id << "\","
            << "\"results\":[";
        
        if (!results.empty()) {
            for (size_t i = 0; i < results.size(); i++) {
                json << results[i].toJson();

                if (i < results.size() - 1) {
                    json << ",";
                }
            }
        }
        json << "]}";

        return json.str();
    }
};

// === RESPONSE DTOs ===

struct TaskDefinitionDto {
    std::string id;
    std::string command;
    std::string args;

    static TaskDefinitionDto fromJson(const std::string& json) {
        TaskDefinitionDto task;
        task.id = Utils::getJsonValue(json, "id");
        task.command = Utils::getJsonValue(json, "command");
        task.args = Utils::getJsonValue(json, "args");

        return task;
    }
};

struct HeartbeatResponseDto {
    int64_t sleepInterval = -1; // -1 indicates no change sent
    int64_t jitterPercent = -1;
    std::vector<TaskDefinitionDto> tasks;

    static HeartbeatResponseDto fromJson(const std::string& json) {
        HeartbeatResponseDto response;

        std::string sleepStr = Utils::getJsonValue(json, "sleep_interval");
        if (!sleepStr.empty()) {
            response.sleepInterval = std::stoi(sleepStr);
        }

        std::string jitterStr = Utils::getJsonValue(json, "jitter");
        if (!jitterStr.empty()) {
            response.jitterPercent = std::stoi(jitterStr);
        }

        std::string tasksArray = Utils::getJsonArrayRaw(json, "tasks");
        if (!tasksArray.empty() && tasksArray != "null") {
            std::vector<std::string> taskObjects = Utils::splitJsonObjects(tasksArray);

            for (const auto& taskJson : taskObjects) {
                response.tasks.push_back(TaskDefinitionDto::fromJson(taskJson));
            }
        }

        return response;
    }
};

#endif