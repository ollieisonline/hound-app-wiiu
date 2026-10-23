#include "video_player.h"
#include <iostream>

VideoPlayer::VideoPlayer(SDL_Renderer* renderer) 
    : renderer(renderer), videoTexture(nullptr), 
      playing(false), paused(false), currentPosition(0.0f), duration(0.0f), decoder(nullptr) {
}

VideoPlayer::~VideoPlayer() {
    cleanup();
}

bool VideoPlayer::init() {
    // Initialize video subsystem
    // For Wii U, we'll use SDL2's texture streaming capabilities
    return true;
}

void VideoPlayer::cleanup() {
    if (videoTexture) {
        SDL_DestroyTexture(videoTexture);
        videoTexture = nullptr;
    }
    
    // Clear frame buffer
    std::lock_guard<std::mutex> lock(frameMutex);
    while (!videoFrames.empty()) {
        delete[] videoFrames.front();
        videoFrames.pop();
    }
    
    if (decoder) {
        // Cleanup decoder
        decoder = nullptr;
    }
}

bool VideoPlayer::openStream(const std::string& url) {
    // For now, just set the URL
    // Actual streaming implementation would go here
    std::cout << "Opening stream: " << url << std::endl;
    
    // Create a placeholder texture
    videoTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        1280,
        720
    );
    
    if (!videoTexture) {
        std::cerr << "Failed to create video texture: " << SDL_GetError() << std::endl;
        return false;
    }
    
    playing = true;
    return true;
}

void VideoPlayer::play() {
    paused = false;
    playing = true;
}

void VideoPlayer::pause() {
    paused = true;
}

void VideoPlayer::stop() {
    playing = false;
    paused = false;
    currentPosition = 0.0f;
    
    cleanup();
}

void VideoPlayer::seek(float position) {
    if (position >= 0.0f && position <= duration) {
        currentPosition = position;
    }
}

bool VideoPlayer::isPlaying() {
    return playing && !paused;
}

bool VideoPlayer::isPaused() {
    return paused;
}

float VideoPlayer::getCurrentPosition() {
    return currentPosition;
}

float VideoPlayer::getDuration() {
    return duration;
}

void VideoPlayer::render(int x, int y, int w, int h) {
    if (!videoTexture) return;
    
    SDL_Rect destRect = {x, y, w, h};
    SDL_RenderCopy(renderer, videoTexture, nullptr, &destRect);
}
