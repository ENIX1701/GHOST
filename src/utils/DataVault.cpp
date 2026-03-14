#include "utils/DataVault.hpp"

std::unordered_map<std::string, std::string> DataVault::vault;
std::mutex DataVault::mtx;

void DataVault::Append(const std::string& key, const std::string& data) {
    std::lock_guard<std::mutex> lock(mtx); // we don't want a race condition, so we lock x3
    vault[key] += data + '\n';
}

std::unordered_map<std::string, std::string> DataVault::Sweep() {
    std::lock_guard<std::mutex> lock(mtx);
    std::unordered_map<std::string, std::string> copy = vault;

    vault.clear();
    
    return copy;
}
