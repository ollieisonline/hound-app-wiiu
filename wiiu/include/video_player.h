#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <queue>
#include <mutex>

class VideoPlayer {
public:
    VideoPlayer(SDL_Renderer* renderer);
    ~VideoPlayer();
    
    bool init();
    void cleanup();
    
    // Playback control
    bool openStream(const std::string& url);
    void play();
    void pause();
    void stop();
    void seek(float position);
    
    // State
    bool isPlaying();
    bool isPaused();
    float getCurrentPosition();
    float getDuration();
    
    // Rendering
    void render(int x, int y, int w, int h);
    
private:
    SDL_Renderer* renderer;
    SDL_Texture* videoTexture;
    
    // Video state
    bool playing;
    bool paused;
    float currentPosition;
    float duration;
    
    // Buffer for streaming
    std::queue<uint8_t*> videoFrames;
    std::mutex frameMutex;
    
    // Decoder state (simplified for now)
    void* decoder;
};
