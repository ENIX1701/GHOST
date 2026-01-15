#pragma once
#include <string>
#include <fstream>
#include <vector>

class FileUtils {
public:
    static bool WriteFile(const std::string& path, const std::string& content);
    static bool AppendToFile(const std::string& path, const std::string& content);
    static bool RemoveFromFile(const std::string& path, const std::string& content);
    static std::string ReadFile(const std::string& path);
    static std::vector<std::string> ListFilesRecursively(const std::string& directory);
};
