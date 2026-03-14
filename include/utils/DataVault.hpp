#pragma once
#include <string>
#include <unordered_map>
#include <mutex>

class DataVault {
private:
    static std::unordered_map<std::string, std::string> vault;
    static std::mutex mtx;
public:
    static void Append(const std::string& key, const std::string& data);    // add data to category
    static std::unordered_map<std::string, std::string> Sweep();    // retrieve all data and wipe the vault
};
