#include "core/Ghost.hpp"
#include "utils/Logger.hpp"

int main() {
    LOG_INFO("GHOST initializing")

    Ghost ghost;

    #ifdef SCENARIO_RANSOMWARE
        ghost.runScenario(Ghost::ScenarioType::RANSOMWARE);
    #elif defined(SCENARIO_ESPIONAGE)
        ghost.runScenario(Ghost::ScenarioType::ESPIONAGE);
    #elif defined(SCENARIO_WIPER)
        ghost.runScenario(Ghost::ScenarioType::WIPER);
    #endif

    #ifndef SCENARIO_MODE
    ghost.run();
    #endif

    return 0;
}
