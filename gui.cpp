#include "gui.h"

bool Application::initialize()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    if (!bGAudio.init())
    {
        cleanup();
        return false;
    }

    window = SDL_CreateWindow(
        "Mafia Character Generator",
        scrW,
        scrH,
        SDL_WINDOW_RESIZABLE
    );

    if (!window)
    {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        cleanup();
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);

    if (!renderer)
    {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        cleanup();
        return false;
    }

    if (!SDL_SetRenderLogicalPresentation(
            renderer,
            scrW,
            scrH,
            SDL_LOGICAL_PRESENTATION_LETTERBOX))
    {
        SDL_Log(
            "SDL_SetRenderLogicalPresentation failed: %s",
            SDL_GetError()
        );

        cleanup();
        return false;
    }

    if (!TTF_Init())
    {
        SDL_Log("TTF_Init failed: %s", SDL_GetError());
        cleanup();
        return false;
    }

    font = TTF_OpenFont(
        "fonts/Merriweather-BlackItalic.ttf",
        32
    );

    if (!font)
    {
        SDL_Log("TTF_OpenFont failed: %s", SDL_GetError());
        cleanup();
        return false;
    }

    bg = imageLoad("images/bg.png");

    if (!bg)
    {
        cleanup();
        return false;
    }

    playerCount = imageLoad("images/playerCount.png");

    if (!playerCount)
    {
        cleanup();
        return false;
    }

    playerCountf = imageLoad("images/playerCountButton.png");

    if (!playerCountf)
    {
        cleanup();
        return false;
    }

    nextB = imageLoad("images/nextB.png");

    if (!nextB)
    {
        cleanup();
        return false;
    }

    exitB = imageLoad("images/exitB.png");

    if (!exitB)
    {
        cleanup();
        return false;
    }

    mafiaCard = imageLoad("images/mafiaCard.png");

    if (!mafiaCard)
    {
        cleanup();
        return false;
    }

    civilianCard = imageLoad("images/civilianCard.png");

    if (!civilianCard)
    {
        cleanup();
        return false;
    }

    countError = imageLoad("images/countError.png");

    if (!countError)
    {
        cleanup();
        return false;
    }

    roleMessage = imageLoad("images/roleMessage.png");

    if (!roleMessage)
    {
        cleanup();
        return false;
    }

    unmuteIcon = imageLoad("images/unmuteIcon.png");

    if (!unmuteIcon)
    {
        cleanup();
        return false;
    }

    muteIcon = imageLoad("images/muteIcon.png");

    if (!muteIcon)
    {
        cleanup();
        return false;
    }

    infoIcon = imageLoad("images/infoIcon.png");

    if (!infoIcon)
    {
        cleanup();
        return false;
    }

    information = imageLoad("images/information.png");

    if (!information)
    {
        cleanup();
        return false;
    }

    infoClose = imageLoad("images/infoCloseButton.png");

    if (!infoClose)
    {
        cleanup();
        return false;
    }

    setupLogicalLayout();

    bGAudio.appBgSound = bGAudio.audioLoad(
        "sounds/bg.mp3"
    );

    if (!bGAudio.appBgSound)
    {
        cleanup();
        return false;
    }

    bGAudio.notifSound = bGAudio.audioLoad(
        "sounds/notif.wav"
    );

    if (!bGAudio.notifSound)
    {
        cleanup();
        return false;
    }

    vsyncEnabled = SDL_SetRenderVSync(renderer, 1);

    if (!vsyncEnabled)
    {
        SDL_Log(
            "Warning: Could not enable VSync: %s",
            SDL_GetError()
        );
    }

    return true;
}

SDL_Texture* Application::imageLoad(const std::string& path)
{
    SDL_Texture* texture =
        IMG_LoadTexture(renderer, path.c_str());

    if (!texture)
    {
        SDL_Log(
            "IMG_LoadTexture failed for %s: %s",
            path.c_str(),
            SDL_GetError()
        );

        return nullptr;
    }

    return texture;
}

void Application::setupLogicalLayout()
{
    playerCountFild.rect = {
        -50.0f,
        330.0f,
        700.0f,
        330.0f
    };

    playerCountButton.rect = {
        136.0f,
        497.0f,
        330.0f,
        55.0f
    };

    exitButton.rect = {
        60.0f,
        632.0f,
        228.0f,
        72.0f
    };

    nextButton.rect = {
        313.0f,
        632.0f,
        228.0f,
        72.0f
    };

    muteButton.rect = {
        10.0f,
        10.0f,
        64.0f,
        64.0f
    };

    infoButton.rect = {
        82.0f,
        10.0f,
        62.0f,
        62.0f
    };

    infoCloseButton.rect = {
    190.0f,
    620.0f,
    220.0f,
    65.0f
    };
}

bool Application::updatePlayerCountTexture()
{
    if (playerCountTxt)
    {
        SDL_DestroyTexture(playerCountTxt);
        playerCountTxt = nullptr;
    }

    if (playerCountText.empty())
    {
        return true;
    }

    SDL_Color textColor = {
        255,
        255,
        255,
        255
    };

    SDL_Surface* surface =
        TTF_RenderText_Blended(
            font,
            playerCountText.c_str(),
            playerCountText.length(),
            textColor
        );

    if (!surface)
    {
        SDL_Log(
            "TTF_RenderText_Blended failed: %s",
            SDL_GetError()
        );

        return false;
    }

    playerCountTxt =
        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );

    SDL_DestroySurface(surface);

    if (!playerCountTxt)
    {
        SDL_Log(
            "SDL_CreateTextureFromSurface failed: %s",
            SDL_GetError()
        );

        return false;
    }

    return true;
}

bool Application::renderBackground()
{
    if (!renderer || !bg)
    {
        return false;
    }

    int outputWidth = 0;
    int outputHeight = 0;

    if (!SDL_GetRenderOutputSize(
            renderer,
            &outputWidth,
            &outputHeight))
    {
        SDL_Log(
            "SDL_GetRenderOutputSize failed: %s",
            SDL_GetError()
        );

        return false;
    }

    if (outputWidth <= 0 || outputHeight <= 0)
    {
        return false;
    }

    float textureWidth = 0.0f;
    float textureHeight = 0.0f;

    if (!SDL_GetTextureSize(
            bg,
            &textureWidth,
            &textureHeight))
    {
        SDL_Log(
            "SDL_GetTextureSize failed: %s",
            SDL_GetError()
        );

        return false;
    }

    if (textureWidth <= 0.0f ||
        textureHeight <= 0.0f)
    {
        return false;
    }

    if (!SDL_SetRenderLogicalPresentation(
            renderer,
            0,
            0,
            SDL_LOGICAL_PRESENTATION_DISABLED))
    {
        SDL_Log(
            "Failed to disable logical presentation: %s",
            SDL_GetError()
        );

        return false;
    }

    const float outputAspect =
        static_cast<float>(outputWidth) /
        static_cast<float>(outputHeight);

    const float textureAspect =
        textureWidth / textureHeight;

    SDL_FRect sourceRect{};

    if (textureAspect > outputAspect)
    {
        const float visibleWidth =
            textureHeight * outputAspect;

        sourceRect.x =
            (textureWidth - visibleWidth) / 2.0f;

        sourceRect.y = 0.0f;
        sourceRect.w = visibleWidth;
        sourceRect.h = textureHeight;
    }
    else
    {
        const float visibleHeight =
            textureWidth / outputAspect;

        sourceRect.x = 0.0f;

        sourceRect.y =
            (textureHeight - visibleHeight) / 2.0f;

        sourceRect.w = textureWidth;
        sourceRect.h = visibleHeight;
    }

    SDL_FRect destinationRect = {
        0.0f,
        0.0f,
        static_cast<float>(outputWidth),
        static_cast<float>(outputHeight)
    };

    const bool rendered =
        SDL_RenderTexture(
            renderer,
            bg,
            &sourceRect,
            &destinationRect
        );

    if (!rendered)
    {
        SDL_Log(
            "Failed to render background: %s",
            SDL_GetError()
        );

        SDL_SetRenderLogicalPresentation(
            renderer,
            scrW,
            scrH,
            SDL_LOGICAL_PRESENTATION_LETTERBOX
        );

        return false;
    }

    if (!SDL_SetRenderLogicalPresentation(
            renderer,
            scrW,
            scrH,
            SDL_LOGICAL_PRESENTATION_LETTERBOX))
    {
        SDL_Log(
            "Failed to restore logical presentation: %s",
            SDL_GetError()
        );

        return false;
    }

    return true;
}

bool Application::renderFullscreenTexture(SDL_Texture* texture)
{
    if (!renderer || !texture)
    {
        return false;
    }

    SDL_FRect destinationRect = {
        0.0f,
        0.0f,
        static_cast<float>(scrW),
        static_cast<float>(scrH)
    };

    if (!SDL_RenderTexture(
            renderer,
            texture,
            nullptr,
            &destinationRect))
    {
        SDL_Log(
            "SDL_RenderTexture failed: %s",
            SDL_GetError()
        );

        return false;
    }

    return true;
}

void Application::cleanup()
{
    if (playerCountTxt)
    {
        SDL_DestroyTexture(playerCountTxt);
        playerCountTxt = nullptr;
    }

    if (bg)
    {
        SDL_DestroyTexture(bg);
        bg = nullptr;
    }

    if (playerCount)
    {
        SDL_DestroyTexture(playerCount);
        playerCount = nullptr;
    }

    if (playerCountf)
    {
        SDL_DestroyTexture(playerCountf);
        playerCountf = nullptr;
    }

    if (exitB)
    {
        SDL_DestroyTexture(exitB);
        exitB = nullptr;
    }

    if (nextB)
    {
        SDL_DestroyTexture(nextB);
        nextB = nullptr;
    }

    if (mafiaCard)
    {
        SDL_DestroyTexture(mafiaCard);
        mafiaCard = nullptr;
    }

    if (civilianCard)
    {
        SDL_DestroyTexture(civilianCard);
        civilianCard = nullptr;
    }

    if (infoIcon)
    {
        SDL_DestroyTexture(infoIcon);
        infoIcon = nullptr;
    }

    if (information)
    {
        SDL_DestroyTexture(information);
        information = nullptr;
    }

    if (infoClose)
    {
        SDL_DestroyTexture(infoClose);
        infoClose = nullptr;
    }

    if (countError)
    {
        SDL_DestroyTexture(countError);
        countError = nullptr;
    }

    if (roleMessage)
    {
        SDL_DestroyTexture(roleMessage);
        roleMessage = nullptr;
    }

    if (unmuteIcon)
    {
        SDL_DestroyTexture(unmuteIcon);
        unmuteIcon = nullptr;
    }

    if (muteIcon)
    {
        SDL_DestroyTexture(muteIcon);
        muteIcon = nullptr;
    }

    bGAudio.cleaner();

    if (font)
    {
        TTF_CloseFont(font);
        font = nullptr;
    }

    if (renderer)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    TTF_Quit();
    SDL_Quit();
}