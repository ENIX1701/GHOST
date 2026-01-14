#pragma once
#include <string>
#include <fstream>
#include <vector>

class FileUtils {
public:
    static bool WriteFile(const std::string& path, const std::string& content) {
        std::ofstream out(path, std::ios::binary);
        if(!out) return false;

        out << content;
        
        return true;
    }

    static bool AppendToFile(const std::string& path, const std::string& content) {
        std::ofstream out(path, std::ios::app);
        if(!out) return false;

        out << "\n" << content;
        
        return true;
    }

    static std::string ReadFile(const std::string& path) {
        std::ifstream in(path, std::ios::binary);
        if (!in) return "";

        return std::string((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    }
};
