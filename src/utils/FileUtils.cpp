#include "utils/FileUtils.hpp"
#include <fstream>
#include <filesystem>

bool FileUtils::WriteFile(const std::string& path, const std::string& content) {
    std::ofstream out(path, std::ios::binary);
    if(!out) return false;

    out << content;
    
    return true;
}

bool FileUtils::AppendToFile(const std::string& path, const std::string& content) {
    std::ofstream out(path, std::ios::app);
    if(!out) return false;

    out << "\n" << content;
    
    return true;
}

bool FileUtils::RemoveFromFile(const std::string& path, const std::string& content) {
    std::vector<std::string> lines;
    std::ifstream in(path);
    if (!in) {
        return false;
    }
    
    std::string line;
    while (std::getline(in, line)) {
        if (line.find(content) == std::string::npos) {
            lines.push_back(line);
        }
    }
    in.close();

    std::ofstream out(path, std::ios::trunc);
    if (!out) {
        return false;
    }

    for (const auto& line : lines) {
        out << line << "\n";
    }
    
    return true;
}

std::string FileUtils::ReadFile(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) return "";

    return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
}

std::vector<std::string> FileUtils::ListFilesRecursively(const std::string& directory) {
    std::vector<std::string> files;

    try {
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                files.push_back(entry.path().string());
            }
        }
    } catch (...) {
        return {};
    }

    return files;
}
