#include "network/Comms.hpp"
#include "core/Config.hpp"
#include "utils/Logger.hpp"
#include <string>
#include <cpr/cpr.h>

static std::string BuildFullUrl(const std::string& endpoint) {
    std::string targetUrl = Config::GetShadowUrl();
    std::string targetPort = Config::GetShadowPort();

    std::string fullUrl;
    if (targetPort.length() > 0 && targetPort != "0") {
        fullUrl = targetUrl + ":" + targetPort + endpoint;
    } else {
        fullUrl = targetUrl + endpoint;
    }

    return fullUrl;
}

std::string Comms::SendPost(const std::string& endpoint, const std::string& payload) {
    std::string fullUrl = BuildFullUrl(endpoint);

    cpr::Response r = cpr::Post(
        cpr::Url{fullUrl},
        cpr::Body{payload},
        cpr::Header{{"Content-Type", "application/json"}, {"User-Agent", Config::GetUserAgent()}},
        cpr::VerifySsl{false}
    );

    if (r.status_code == 200) {
        return r.text;
    } else {
        LOG_ERROR("HTTP error {}: {}", r.status_code, r.error.message)
        return "";
    }
}

bool Comms::UploadFile(const std::string& endpoint, const std::string& filename, const std::string& fileContent) {
    std::string fullUrl = BuildFullUrl(endpoint);

    cpr::Response r = cpr::Post(
        cpr::Url{fullUrl},
        cpr::Multipart{{"file", cpr::File(filename, fileContent)}},
        cpr::Header{{"User-Agent", Config::GetUserAgent()}},
        cpr::VerifySsl{false}
    );

    return r.status_code == 200;
}
