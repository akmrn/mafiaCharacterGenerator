#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_ttf/SDL_ttf.h>

#include <string>

#include "button.h"
#include "audio.h"

struct Application
{
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;

    SDL_Texture* bg = nullptr;
    SDL_Texture* playerCount = nullptr;
    SDL_Texture* playerCountTxt = nullptr;
    SDL_Texture* playerCountf = nullptr;
    SDL_Texture* exitB = nullptr;
    SDL_Texture* nextB = nullptr;
    SDL_Texture* mafiaCard = nullptr;
    SDL_Texture* civilianCard = nullptr;
    SDL_Texture* countError = nullptr;
    SDL_Texture* roleMessage = nullptr;
    SDL_Texture* unmuteIcon = nullptr;
    SDL_Texture* muteIcon = nullptr;
    SDL_Texture* infoIcon = nullptr;
    SDL_Texture* information = nullptr;
    SDL_Texture* infoClose = nullptr;
    
    Button playerCountFild;
    Button playerCountButton;
    Button exitButton;
    Button nextButton;
    Button muteButton;
    Button infoButton;
    Button infoCloseButton;

    Audio bGAudio;

    std::string playerCountText;

    bool vsyncEnabled = false;
    bool playerCountActive = false;

    bool initialize();

    SDL_Texture* imageLoad(const std::string& path);

    void cleanup();

    bool updatePlayerCountTexture();

    void setupLogicalLayout();

    bool renderBackground();

    bool renderFullscreenTexture(SDL_Texture* texture);

    static constexpr int scrW = 600;
    static constexpr int scrH = 800;
};