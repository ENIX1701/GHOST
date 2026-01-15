#include "core/Ghost.hpp"
#include "utils/Logger.hpp"

int main() {
    LOG_INFO("GHOST initializing")

    Ghost ghost;
    ghost.run();

    return 0;
}
