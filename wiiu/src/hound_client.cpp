#include "hound_client.h"
#include "network.h"
#include <iostream>
#include <sstream>
#include <algorithm>

HoundClient::HoundClient() : serverPort(8080), connected(false) {}

HoundClient::~HoundClient() {
    disconnect();
}

bool HoundClient::connect(const std::string& host, int port) {
    serverHost = host;
    serverPort = port;
    
    NetworkManager network;
    if (!network.init()) {
        return false;
    }
    
    connected = network.connectToServer(host, port);
    return connected;
}

void HoundClient::disconnect() {
    connected = false;
    sessionToken.clear();
    currentProfileId.clear();
}

bool HoundClient::isConnected() {
    return connected;
}

std::vector<MediaItem> HoundClient::getMediaList() {
    std::vector<MediaItem> result;
    
    NetworkManager network;
    if (!network.init()) {
        return result;
    }
    
    std::string response = network.httpGet("http://" + serverHost + ":" + std::to_string(serverPort) + "/api/media");
    
    // Parse JSON response (simplified - in real implementation use a JSON parser)
    // This is a placeholder for actual JSON parsing
    
    // Example: [{"id": "1", "title": "Movie 1", ...}, ...]
    // For now, return dummy data
    for (int i = 0; i < 10; i++) {
        MediaItem item;
        item.id = "media_" + std::to_string(i);
        item.title = "Sample Media " + std::to_string(i);
        item.type = "movie";
        item.overview = "This is a sample media item";
        item.rating = 7.5f;
        result.push_back(item);
    }
    
    return result;
}

MediaItem HoundClient::getMediaInfo(const std::string& id) {
    MediaItem item;
    item.id = id;
    item.title = "Sample Media";
    item.type = "movie";
    item.overview = "Detailed information about this media item";
    item.rating = 8.0f;
    item.releaseDate = "2024-01-01";
    item.genres = {"Action", "Adventure"};
    return item;
}

std::vector<MediaItem> HoundClient::searchMedia(const std::string& query) {
    std::vector<MediaItem> result;
    
    NetworkManager network;
    if (!network.init()) {
        return result;
    }
    
    std::string response = network.httpGet("http://" + serverHost + ":" + std::to_string(serverPort) + "/api/search?q=" + query);
    
    // Parse search results
    // Placeholder: return dummy data
    for (int i = 0; i < 5; i++) {
        MediaItem item;
        item.id = "search_" + std::to_string(i);
        item.title = "Search Result " + std::to_string(i) + " for " + query;
        item.type = "movie";
        result.push_back(item);
    }
    
    return result;
}

std::vector<Collection> HoundClient::getCollections() {
    std::vector<Collection> result;
    
    NetworkManager network;
    if (!network.init()) {
        return result;
    }
    
    std::string response = network.httpGet("http://" + serverHost + ":" + std::to_string(serverPort) + "/api/collections");
    
    // Parse collections
    // Placeholder: return dummy data
    for (int i = 0; i < 3; i++) {
        Collection collection;
        collection.id = "collection_" + std::to_string(i);
        collection.name = "Collection " + std::to_string(i);
        collection.description = "Sample collection";
        collection.mediaIds = {"media_0", "media_1", "media_2"};
        result.push_back(collection);
    }
    
    return result;
}

Collection HoundClient::getCollectionInfo(const std::string& id) {
    Collection collection;
    collection.id = id;
    collection.name = "Sample Collection";
    collection.description = "This is a sample collection";
    collection.mediaIds = {"media_0", "media_1", "media_2"};
    return collection;
}

std::string HoundClient::getStreamUrl(const std::string& mediaId) {
    return "http://" + serverHost + ":" + std::to_string(serverPort) + "/stream/" + mediaId;
}

bool HoundClient::login(const std::string& username, const std::string& password) {
    NetworkManager network;
    if (!network.init()) {
        return false;
    }
    
    std::string authData = "{" \"username\": \"" + username + "\", \"password\": \"" + password + "\" }";
    std::string response = network.httpPost("http://" + serverHost + ":" + std::to_string(serverPort) + "/api/login", authData);
    
    // Parse response for session token
    // Placeholder: assume success
    sessionToken = "sample_token";
    return true;
}

bool HoundClient::selectProfile(const std::string& profileId) {
    currentProfileId = profileId;
    return true;
}
