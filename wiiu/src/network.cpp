#include "network.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <cstring>

NetworkManager::NetworkManager() : socket(nullptr), udpSocket(nullptr) {}

NetworkManager::~NetworkManager() {
    cleanup();
}

bool NetworkManager::init() {
    if (SDLNet_Init() == -1) {
        std::cerr << "SDLNet_Init: " << SDLNet_GetError() << std::endl;
        return false;
    }
    return true;
}

void NetworkManager::cleanup() {
    if (socket) {
        SDLNet_TCP_Close(socket);
        socket = nullptr;
    }
    if (udpSocket) {
        SDLNet_UDP_Close(udpSocket);
        udpSocket = nullptr;
    }
    SDLNet_Quit();
}

std::string NetworkManager::httpGet(const std::string& url) {
    // Parse URL
    size_t protocolPos = url.find("://");
    if (protocolPos == std::string::npos) {
        return "";
    }
    
    size_t pathPos = url.find('/', protocolPos + 3);
    std::string host = url.substr(protocolPos + 3, pathPos - (protocolPos + 3));
    std::string path = (pathPos != std::string::npos) ? url.substr(pathPos) : "/";
    
    // Resolve host
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, host.c_str(), 80) < 0) {
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        return "";
    }
    
    // Open socket
    TCPsocket sock = SDLNet_TCP_Open(&ip);
    if (!sock) {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        return "";
    }
    
    // Send HTTP request
    std::string request = "GET " + path + " HTTP/1.1\r\n";
    request += "Host: " + host + "\r\n";
    request += "Connection: close\r\n\r\n";
    
    int sent = SDLNet_TCP_Send(sock, request.c_str(), request.size());
    if (sent < request.size()) {
        std::cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << std::endl;
        SDLNet_TCP_Close(sock);
        return "";
    }
    
    // Receive response
    std::string response;
    char buffer[4096];
    int received;
    
    while ((received = SDLNet_TCP_Recv(sock, buffer, sizeof(buffer))) > 0) {
        response.append(buffer, received);
    }
    
    SDLNet_TCP_Close(sock);
    
    // Extract body from HTTP response
    size_t headerEnd = response.find("\r\n\r\n");
    if (headerEnd != std::string::npos) {
        return response.substr(headerEnd + 4);
    }
    
    return response;
}

std::string NetworkManager::httpPost(const std::string& url, const std::string& data) {
    // Similar to httpGet but with POST method
    size_t protocolPos = url.find("://");
    if (protocolPos == std::string::npos) {
        return "";
    }
    
    size_t pathPos = url.find('/', protocolPos + 3);
    std::string host = url.substr(protocolPos + 3, pathPos - (protocolPos + 3));
    std::string path = (pathPos != std::string::npos) ? url.substr(pathPos) : "/";
    
    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, host.c_str(), 80) < 0) {
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        return "";
    }
    
    TCPsocket sock = SDLNet_TCP_Open(&ip);
    if (!sock) {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        return "";
    }
    
    std::string request = "POST " + path + " HTTP/1.1\r\n";
    request += "Host: " + host + "\r\n";
    request += "Content-Type: application/json\r\n";
    request += "Content-Length: " + std::to_string(data.size()) + "\r\n";
    request += "Connection: close\r\n\r\n";
    request += data;
    
    int sent = SDLNet_TCP_Send(sock, request.c_str(), request.size());
    if (sent < request.size()) {
        std::cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << std::endl;
        SDLNet_TCP_Close(sock);
        return "";
    }
    
    std::string response;
    char buffer[4096];
    int received;
    
    while ((received = SDLNet_TCP_Recv(sock, buffer, sizeof(buffer))) > 0) {
        response.append(buffer, received);
    }
    
    SDLNet_TCP_Close(sock);
    
    size_t headerEnd = response.find("\r\n\r\n");
    if (headerEnd != std::string::npos) {
        return response.substr(headerEnd + 4);
    }
    
    return response;
}

bool NetworkManager::connectToServer(const std::string& host, int port) {
    if (SDLNet_ResolveHost(&serverAddress, host.c_str(), port) < 0) {
        std::cerr << "SDLNet_ResolveHost: " << SDLNet_GetError() << std::endl;
        return false;
    }
    
    socket = SDLNet_TCP_Open(&serverAddress);
    if (!socket) {
        std::cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << std::endl;
        return false;
    }
    
    return true;
}

std::string NetworkManager::getMediaList() {
    // API endpoint for media list
    char ipStr[256];
    SDLNet_GetIPString(&serverAddress, ipStr, sizeof(ipStr));
    return httpGet("http://" + std::string(ipStr) + ":8080/api/media");
}

std::string NetworkManager::getMediaInfo(const std::string& id) {
    char ipStr[256];
    SDLNet_GetIPString(&serverAddress, ipStr, sizeof(ipStr));
    return httpGet("http://" + std::string(ipStr) + ":8080/api/media/" + id);
}

std::string NetworkManager::streamMedia(const std::string& id) {
    char ipStr[256];
    SDLNet_GetIPString(&serverAddress, ipStr, sizeof(ipStr));
    return httpGet("http://" + std::string(ipStr) + ":8080/stream/" + id);
}

bool NetworkManager::connectWebSocket(const std::string& url) {
    // WebSocket implementation would go here
    // For simplicity, we'll use a basic TCP socket
    return connectToServer(url, 8080);
}

void NetworkManager::sendWebSocketMessage(const std::string& message) {
    if (socket) {
        // Simple WebSocket framing (simplified)
        std::string framed = "\x81"; // Text frame
        framed += static_cast<char>(0x80 | (message.size() & 0x7F));
        framed += message;
        
        SDLNet_TCP_Send(socket, framed.c_str(), framed.size());
    }
}

void NetworkManager::setWebSocketCallback(std::function<void(const std::string&)> callback) {
    webSocketCallback = callback;
}
