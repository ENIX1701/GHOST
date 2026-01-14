#include "core/Ghost.hpp"
#include "utils/Logger.hpp"
#include <thread>
#include <chrono>

int main() {
    LOG_INFO("GHOST initializing")

    Ghost ghost;
    ghost.run();

    return 0;
}
