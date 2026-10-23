#pragma once

#include <string>
#include <vector>
#include <map>

// Media item structure
struct MediaItem {
    std::string id;
    std::string title;
    std::string type; // "movie", "tv", "music", etc.
    std::string posterUrl;
    std::string backdropUrl;
    std::string overview;
    float rating;
    std::string releaseDate;
    std::vector<std::string> genres;
};

// Collection structure
struct Collection {
    std::string id;
    std::string name;
    std::string description;
    std::string posterUrl;
    std::vector<std::string> mediaIds;
};

class HoundClient {
public:
    HoundClient();
    ~HoundClient();
    
    // Server connection
    bool connect(const std::string& host, int port = 8080);
    void disconnect();
    bool isConnected();
    
    // Media operations
    std::vector<MediaItem> getMediaList();
    MediaItem getMediaInfo(const std::string& id);
    std::vector<MediaItem> searchMedia(const std::string& query);
    
    // Collection operations
    std::vector<Collection> getCollections();
    Collection getCollectionInfo(const std::string& id);
    
    // Streaming
    std::string getStreamUrl(const std::string& mediaId);
    
    // User/Session
    bool login(const std::string& username, const std::string& password);
    bool selectProfile(const std::string& profileId);
    
private:
    std::string serverHost;
    int serverPort;
    bool connected;
    std::string sessionToken;
    std::string currentProfileId;
};
