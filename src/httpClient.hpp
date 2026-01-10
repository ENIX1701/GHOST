#ifndef HTTP_CLIENT_HPP
#define HTTP_CLIENT_HPP

#define SOCKET_TYPE int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define IS_VALID_SOCKET(s) (s >= 0)
#define CLOSE_SOCKET close

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
        struct sockaddr_in server_addr{};
        server_addr.sin_family = AF_INET;
        inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr);
        server_addr.sin_port = htons(port);

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
        } else {
            return response;
        }

        return "";
    }
};

#endif