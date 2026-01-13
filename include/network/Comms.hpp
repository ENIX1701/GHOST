#pragma once
#include <string>
#include <cpr/cpr.h>

#include "core/Config.hpp"
#include "utils/Logger.hpp"

class Comms {
public:
    static std::string SendPost(const std::string& endpoint, const std::string& payload) {
        std::string fullUrl = Config::SHADOW_URL + endpoint;

        cpr::Response r = cpr::Post(
            cpr::Url{fullUrl},
            cpr::Body{payload},
            cpr::Header{{"Content-Type", "application/json"}, {"User-Agent", Config::USER_AGENT}},
            cpr::VerifySsl{false}
        );

        if (r.status_code == 200) {
            return r.text;
        } else {
            LOG_ERROR("HTTP error {}: {}", r.status_code, r.error.message)
            return "";
        }
    }

    static bool UploadFile(const std::string& endpoint, const std::string& filename, const std::string& fileContent) {
        std::string fullUrl = Config::SHADOW_URL + endpoint;

        cpr::Response r = cpr::Post(
            cpr::Url{fullUrl},
            cpr::Multipart{{"file": cpr::File(filename, fileContent)}},
            cpr::Header{{"User-Agent", Config::USER_AGENT}}
        );

        return r.status_code == 200;
    }
};
