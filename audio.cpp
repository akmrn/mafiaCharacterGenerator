#include "audio.h"

bool Audio::init()
{
    // Initialize SDL_mixer
    if (!MIX_Init())
    {
        SDL_Log("Mix_Init error: %s", SDL_GetError());
        return false;
    }

    mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
    if (!mixer)
    {
        SDL_Log("MIX_CreateMixerDevice failed: %s", SDL_GetError());
        cleaner();
        return false;
    }

    return true;
}

MIX_Audio* Audio::audioLoad(const std::string& path)
{
    MIX_Audio* sound = MIX_LoadAudio(mixer, path.c_str(), false);
    if (!sound)
    {
        SDL_Log("MIX_LoadAudio failed: %s", SDL_GetError());

        return nullptr;
    }

    return sound;
}

bool Audio::audioPlay(MIX_Audio* sound)
{
    if (!sound)
    {
        SDL_Log("No music loaded!");
        return false;
    }

    if (!MIX_PlayAudio(mixer, sound))
    {
        SDL_Log("Mix_PlayMusic error: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool Audio::audioPlayLoop(MIX_Audio* sound)
{
    if (!sound)
    {
        SDL_Log("No audio loaded!");
        return false;
    }

    if (!mixer)
    {
        SDL_Log("Mixer is null!");
        return false;
    }

    if (!bgTrack)
    {
        bgTrack = MIX_CreateTrack(mixer);

        if (!bgTrack)
        {
            SDL_Log("MIX_CreateTrack failed: %s", SDL_GetError());
            return false;
        }
    }

    if (!MIX_SetTrackAudio(bgTrack, sound))
    {
        SDL_Log("MIX_SetTrackAudio failed: %s", SDL_GetError());
        return false;
    }

    SDL_PropertiesID options = SDL_CreateProperties();

    if (!options)
    {
        SDL_Log("SDL_CreateProperties failed: %s", SDL_GetError());
        return false;
    }

    // -1 = loop forever
    if (!SDL_SetNumberProperty(
            options,
            MIX_PROP_PLAY_LOOPS_NUMBER,
            -1))
    {
        SDL_Log("SDL_SetNumberProperty failed: %s", SDL_GetError());

        SDL_DestroyProperties(options);
        return false;
    }

    bool result = MIX_PlayTrack(bgTrack, options);

    SDL_DestroyProperties(options);

    if (!result)
    {
        SDL_Log("MIX_PlayTrack failed: %s", SDL_GetError());
        return false;
    }

    return true;
}

bool Audio::mute()
{
    if (!mixer)
    {
        SDL_Log("Cannot mute: mixer is null.");
        return false;
    }

    if (!MIX_SetMixerGain(mixer, 0.0f))
    {
        SDL_Log(
            "MIX_SetMixerGain mute failed: %s",
            SDL_GetError()
        );

        return false;
    }

    return true;
}

bool Audio::unmute()
{
    if (!mixer)
    {
        SDL_Log("Cannot unmute: mixer is null.");
        return false;
    }

    if (!MIX_SetMixerGain(mixer, 1.0f))
    {
        SDL_Log(
            "MIX_SetMixerGain unmute failed: %s",
            SDL_GetError()
        );

        return false;
    }

    return true;
}

void Audio::cleaner()
{
    if (bgTrack)
    {
        MIX_DestroyTrack(bgTrack);
        bgTrack = nullptr;
    }

    if (appBgSound)
    {
        MIX_DestroyAudio(appBgSound);
        appBgSound = nullptr;
    }

    if (notifSound)
    {
        MIX_DestroyAudio(notifSound);
        notifSound = nullptr;
    }

    if (mixer)
    {
        MIX_DestroyMixer(mixer);
        mixer = nullptr;
    }

    MIX_Quit();
}