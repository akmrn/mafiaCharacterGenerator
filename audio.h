#pragma once

#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <string>

struct Audio
{
    MIX_Mixer* mixer = nullptr;
    MIX_Audio* appBgSound = nullptr;
    MIX_Audio* notifSound = nullptr;

    MIX_Track* bgTrack = nullptr;

    bool init();
    MIX_Audio* audioLoad(const std::string& path);
    bool audioPlay(MIX_Audio* sound);

    bool audioPlayLoop(MIX_Audio* sound);

    bool mute();
    bool unmute();

    void cleaner();
};