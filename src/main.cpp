#include "core/Ghost.hpp"
#include "utils/Logger.hpp"

int main() {
    LOG_INFO("GHOST initializing")

    Ghost ghost;

    #ifdef SCENARIO_RANSOMWARE
        ghost.runScenario(Ghost::ScenarioType::RANSOMWARE);
    #elif defined(SCENARIO_ESPIONAGE)
        ghost.runScenario(Ghost::ScenarioType::ESPIONAGE);
    #endif

    // TODO: make this run only in non-scenario builds
    ghost.run();

    return 0;
}
