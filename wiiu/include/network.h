#pragma once

#include <SDL2/SDL_net.h>
#include <string>
#include <functional>
#include <map>

class NetworkManager {
public:
    NetworkManager();
    ~NetworkManager();
    
    bool init();
    void cleanup();
    
    // HTTP requests
    std::string httpGet(const std::string& url);
    std::string httpPost(const std::string& url, const std::string& data);
    
    // Hound API specific
    bool connectToServer(const std::string& host, int port = 8080);
    std::string getMediaList();
    std::string getMediaInfo(const std::string& id);
    std::string streamMedia(const std::string& id);
    
    // WebSocket for real-time updates
    bool connectWebSocket(const std::string& url);
    void sendWebSocketMessage(const std::string& message);
    void setWebSocketCallback(std::function<void(const std::string&)> callback);
    
private:
    IPaddress serverAddress;
    TCPsocket socket;
    UDPsocket udpSocket;
    
    std::function<void(const std::string&)> webSocketCallback;
};
