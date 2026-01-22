#include "utils/UuidUtils.hpp"
#include <string>
#include <random>
#include <chrono>
#include <sstream>
#include <iomanip>
#include <array>
#include <cstdint>

std::string UuidUtils::Generate() {
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    uint64_t millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();

    std::array<uint8_t, 16> uuid;
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint8_t> dist(0, 255);

    for (auto& e : uuid) {
        e = dist(gen);
    }

    uuid[0] = (millis >> 40) & 0xFF;
    uuid[1] = (millis >> 32) & 0xFF;
    uuid[2] = (millis >> 24) & 0xFF;
    uuid[3] = (millis >> 16) & 0xFF;
    uuid[4] = (millis >> 8) & 0xFF;
    uuid[5] = millis & 0xFF;

    uuid[6] = (uuid[6] & 0x0F) | 0x70;
    uuid[8] = (uuid[8] & 0x3F) | 0x80;

    std::stringstream ss;
    ss << std::hex << std::setfill('0');

    ss << std::setw(2) << (int)uuid[0] << std::setw(2) << (int)uuid[1]
        << std::setw(2) << (int)uuid[2] << std::setw(2) << (int)uuid[3]
        << "-"
        << std::setw(2) << (int)uuid[4] << std::setw(2) << (int)uuid[5]
        << "-"
        << std::setw(2) << (int)uuid[6] << std::setw(2) << (int)uuid[7]
        << "-"
        << std::setw(2) << (int)uuid[8] << std::setw(2) << (int)uuid[9]
        << "-"
        << std::setw(2) << (int)uuid[10] << std::setw(2) << (int)uuid[11]
        << std::setw(2) << (int)uuid[12] << std::setw(2) << (int)uuid[13]
        << std::setw(2) << (int)uuid[14] << std::setw(2) << (int)uuid[15];

    return ss.str(); 
}
