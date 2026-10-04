#include "windowLoop.h"
#include "gui.h"
#include "gameLogic.h"
#include "audio.h"

#include <algorithm>
#include <cstdlib>

namespace
{
    bool convertEventToLogicalCoordinates(
        SDL_Renderer* renderer,
        SDL_Event& event)
    {
        if (!renderer)
        {
            return false;
        }

        if (!SDL_ConvertEventToRenderCoordinates(
                renderer,
                &event))
        {
            SDL_Log(
                "SDL_ConvertEventToRenderCoordinates failed: %s",
                SDL_GetError()
            );

            return false;
        }

        return true;
    }

    bool getPhysicalCoordinates(
        Application& app,
        const SDL_Event& event,
        float& x,
        float& y)
    {
        if (!app.renderer || !app.window)
        {
            return false;
        }

        int outputWidth = 0;
        int outputHeight = 0;

        if (!SDL_GetRenderOutputSize(
                app.renderer,
                &outputWidth,
                &outputHeight))
        {
            return false;
        }

        if (outputWidth <= 0 || outputHeight <= 0)
        {
            return false;
        }

        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            int windowWidth = 0;
            int windowHeight = 0;

            if (!SDL_GetWindowSize(
                    app.window,
                    &windowWidth,
                    &windowHeight))
            {
                return false;
            }

            if (windowWidth <= 0 || windowHeight <= 0)
            {
                return false;
            }

            x =
                event.button.x *
                static_cast<float>(outputWidth) /
                static_cast<float>(windowWidth);

            y =
                event.button.y *
                static_cast<float>(outputHeight) /
                static_cast<float>(windowHeight);

            return true;
        }

        if (event.type == SDL_EVENT_FINGER_DOWN)
        {
            x =
                event.tfinger.x *
                static_cast<float>(outputWidth);

            y =
                event.tfinger.y *
                static_cast<float>(outputHeight);

            return true;
        }

        return false;
    }

    SDL_FRect getPhysicalMuteRect(Application& app)
    {
        SDL_FRect result = {
            0.0f,
            0.0f,
            0.0f,
            0.0f
        };

        if (!app.renderer)
        {
            return result;
        }

        int outputWidth = 0;
        int outputHeight = 0;

        if (!SDL_GetRenderOutputSize(
                app.renderer,
                &outputWidth,
                &outputHeight))
        {
            return result;
        }

        if (outputWidth <= 0 || outputHeight <= 0)
        {
            return result;
        }

        const float scaleX =
            static_cast<float>(outputWidth) /
            static_cast<float>(Application::scrW);

        const float scaleY =
            static_cast<float>(outputHeight) /
            static_cast<float>(Application::scrH);

        const float scale =
            std::min(scaleX, scaleY);

        const float width =
            app.muteButton.rect.w * scale;

        const float height =
            app.muteButton.rect.h * scale;

        const float x =
            app.muteButton.rect.x * scale;

        const float y =
            app.muteButton.rect.y * scale;

        result = {
            x,
            y,
            width,
            height
        };

        return result;
    }

    SDL_FRect getPhysicalInfoRect(Application& app)
    {
        SDL_FRect result = {
            0.0f,
            0.0f,
            0.0f,
            0.0f
        };

        if (!app.renderer)
        {
            return result;
        }

        int outputWidth = 0;
        int outputHeight = 0;

        if (!SDL_GetRenderOutputSize(
                app.renderer,
                &outputWidth,
                &outputHeight))
        {
            return result;
        }

        if (outputWidth <= 0 || outputHeight <= 0)
        {
            return result;
        }

        const float scaleX =
            static_cast<float>(outputWidth) /
            static_cast<float>(Application::scrW);

        const float scaleY =
            static_cast<float>(outputHeight) /
            static_cast<float>(Application::scrH);

        const float scale =
            std::min(scaleX, scaleY);

        const float width =
            app.infoButton.rect.w * scale;

        const float height =
            app.infoButton.rect.h * scale;

        const float x =
            app.infoButton.rect.x * scale;

        const float y =
            app.infoButton.rect.y * scale;

        result = {
            x,
            y,
            width,
            height
        };

        return result;
    }

    bool isInsideRect(
        const SDL_FRect& rect,
        float x,
        float y)
    {
        return x >= rect.x &&
               x <= rect.x + rect.w &&
               y >= rect.y &&
               y <= rect.y + rect.h;
    }

    bool isMuteButtonPressed(
        Application& app,
        const SDL_Event& event)
    {
        float physicalX = 0.0f;
        float physicalY = 0.0f;

        if (!getPhysicalCoordinates(
                app,
                event,
                physicalX,
                physicalY))
        {
            return false;
        }

        const SDL_FRect muteRect =
            getPhysicalMuteRect(app);

        return isInsideRect(
            muteRect,
            physicalX,
            physicalY
        );
    }

    bool isInfoButtonPressed(
        Application& app,
        const SDL_Event& event)
    {
        float physicalX = 0.0f;
        float physicalY = 0.0f;

        if (!getPhysicalCoordinates(
                app,
                event,
                physicalX,
                physicalY))
        {
            return false;
        }

        const SDL_FRect infoRect =
            getPhysicalInfoRect(app);

        return isInsideRect(
            infoRect,
            physicalX,
            physicalY
        );
    }

    bool renderMuteButton(
        Application& app,
        bool muted)
    {
        if (!app.renderer)
        {
            return false;
        }

        SDL_Texture* texture =
            muted
                ? app.muteIcon
                : app.unmuteIcon;

        if (!texture)
        {
            return false;
        }

        const SDL_FRect muteRect =
            getPhysicalMuteRect(app);

        if (muteRect.w <= 0.0f ||
            muteRect.h <= 0.0f)
        {
            return false;
        }

        if (!SDL_SetRenderLogicalPresentation(
                app.renderer,
                0,
                0,
                SDL_LOGICAL_PRESENTATION_DISABLED))
        {
            SDL_Log(
                "Failed to disable logical presentation for mute button: %s",
                SDL_GetError()
            );

            return false;
        }

        const bool rendered =
            SDL_RenderTexture(
                app.renderer,
                texture,
                nullptr,
                &muteRect
            );

        if (!rendered)
        {
            SDL_Log(
                "Failed to render mute button: %s",
                SDL_GetError()
            );
        }

        if (!SDL_SetRenderLogicalPresentation(
                app.renderer,
                Application::scrW,
                Application::scrH,
                SDL_LOGICAL_PRESENTATION_LETTERBOX))
        {
            SDL_Log(
                "Failed to restore logical presentation after mute button: %s",
                SDL_GetError()
            );

            return false;
        }

        return rendered;
    }

    bool renderInfoButton(Application& app)
    {
        if (!app.renderer || !app.infoIcon)
        {
            return false;
        }

        const SDL_FRect infoRect =
            getPhysicalInfoRect(app);

        if (infoRect.w <= 0.0f ||
            infoRect.h <= 0.0f)
        {
            return false;
        }

        if (!SDL_SetRenderLogicalPresentation(
                app.renderer,
                0,
                0,
                SDL_LOGICAL_PRESENTATION_DISABLED))
        {
            SDL_Log(
                "Failed to disable logical presentation for info button: %s",
                SDL_GetError()
            );

            return false;
        }

        const bool rendered =
            SDL_RenderTexture(
                app.renderer,
                app.infoIcon,
                nullptr,
                &infoRect
            );

        if (!rendered)
        {
            SDL_Log(
                "Failed to render info button: %s",
                SDL_GetError()
            );
        }

        if (!SDL_SetRenderLogicalPresentation(
                app.renderer,
                Application::scrW,
                Application::scrH,
                SDL_LOGICAL_PRESENTATION_LETTERBOX))
        {
            SDL_Log(
                "Failed to restore logical presentation after info button: %s",
                SDL_GetError()
            );

            return false;
        }

        return rendered;
    }

    bool isInfoCloseButtonPressed(
        Application& app,
        const SDL_Event& event)
    {
        if (!app.renderer)
        {
            return false;
        }

        SDL_Event logicalEvent = event;

        if (!convertEventToLogicalCoordinates(
                app.renderer,
                logicalEvent))
        {
            return false;
        }

        float x = 0.0f;
        float y = 0.0f;

        if (logicalEvent.type ==
            SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            x = logicalEvent.button.x;
            y = logicalEvent.button.y;
        }
        else if (logicalEvent.type ==
                 SDL_EVENT_FINGER_DOWN)
        {
            x = logicalEvent.tfinger.x;
            y = logicalEvent.tfinger.y;
        }
        else
        {
            return false;
        }

        return app.infoCloseButton.isMouseInside(
            x,
            y
        );
    }

    void handleMute(
        Application& app,
        bool& audioMuted,
        const SDL_Event& event)
    {
        if (!isMuteButtonPressed(app, event))
        {
            return;
        }

        const bool newMutedState =
            !audioMuted;

        if (newMutedState)
        {
            if (app.bGAudio.mute())
            {
                audioMuted = true;
            }
            else
            {
                SDL_Log("Failed to mute audio.");
            }
        }
        else
        {
            if (app.bGAudio.unmute())
            {
                audioMuted = false;
            }
            else
            {
                SDL_Log("Failed to unmute audio.");
            }
        }
    }

    void handleInfo(
        Application& app,
        Window& window,
        const SDL_Event& event)
    {
        if (window.informationVisible)
        {
            if (isInfoCloseButtonPressed(
                    app,
                    event))
            {
                window.informationVisible = false;
            }

            return;
        }

        if (isInfoButtonPressed(
                app,
                event))
        {
            window.informationVisible = true;

            app.playerCountActive = false;

            SDL_StopTextInput(
                app.window
            );
        }
    }

    bool handleNextButton(
        Application& app,
        GameLog& game,
        Window& window)
    {
        if (window.gameState == GameState::PlayerCount)
        {
            if (app.playerCountText.empty())
            {
                return true;
            }

            int count = 0;

            try
            {
                count =
                    std::stoi(
                        app.playerCountText
                    );
            }
            catch (...)
            {
                window.isDisplayError = true;

                app.bGAudio.audioPlay(
                    app.bGAudio.notifSound
                );

                return true;
            }

            if (!game.playerCountRule(count))
            {
                SDL_Log(
                    "Player count must be between 3 and 50."
                );

                window.isDisplayError = true;

                app.bGAudio.audioPlay(
                    app.bGAudio.notifSound
                );

                return true;
            }

            window.playerCount = count;

            window.roles =
                game.generateRoles(
                    window.playerCount
                );

            window.isDisplay = false;
            window.isDisplayError = false;

            window.gameState =
                GameState::ShowPlayerMessage;

            app.bGAudio.audioPlay(
                app.bGAudio.notifSound
            );

            return true;
        }

        if (window.gameState ==
            GameState::ShowPlayerMessage)
        {
            window.gameState =
                GameState::ShowRole;

            return true;
        }

        if (window.gameState ==
            GameState::ShowRole)
        {
            ++window.currentPlayer;

            if (window.currentPlayer <
                window.playerCount)
            {
                window.gameState =
                    GameState::ShowPlayerMessage;

                app.bGAudio.audioPlay(
                    app.bGAudio.notifSound
                );
            }
            else
            {
                SDL_Log(
                    "All players saw their roles."
                );

                window.gameState =
                    GameState::Finished;
            }

            return true;
        }

        if (window.gameState ==
            GameState::Finished)
        {
            window.gameState =
                GameState::PlayerCount;

            window.currentPlayer = 0;
            window.playerCount = 0;

            window.roles.clear();

            window.isDisplay = true;
            window.isDisplayError = false;
            window.informationVisible = false;

            app.playerCountText.clear();

            app.updatePlayerCountTexture();

            return true;
        }

        return true;
    }

    void handleLogicalPointerEvent(
        Application& app,
        GameLog& game,
        Window& window,
        SDL_Event event)
    {
        if (window.informationVisible)
        {
            if (isInfoCloseButtonPressed(
                    app,
                    event))
            {
                window.informationVisible = false;
            }

            return;
        }

        if (!convertEventToLogicalCoordinates(
                app.renderer,
                event))
        {
            return;
        }

        float x = 0.0f;
        float y = 0.0f;

        if (event.type ==
            SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            x = event.button.x;
            y = event.button.y;
        }
        else if (event.type ==
                 SDL_EVENT_FINGER_DOWN)
        {
            x = event.tfinger.x;
            y = event.tfinger.y;
        }
        else
        {
            return;
        }

        if (window.isDisplay &&
            app.playerCountButton.isMouseInside(
                x,
                y))
        {
            app.playerCountActive = true;

            SDL_StartTextInput(
                app.window
            );
        }
        else
        {
            app.playerCountActive = false;

            SDL_StopTextInput(
                app.window
            );
        }

        if (window.isDisplayButton &&
            app.exitButton.isMouseInside(
                x,
                y))
        {
            window.running = false;
            return;
        }

        if (window.isDisplayButton &&
            app.nextButton.isMouseInside(
                x,
                y))
        {
            handleNextButton(
                app,
                game,
                window
            );
        }
    }

    void renderPlayerCountScreen(
        Application& app)
    {
        if (!app.playerCount)
        {
            return;
        }

        if (!SDL_RenderTexture(
                app.renderer,
                app.playerCount,
                nullptr,
                &app.playerCountFild.rect))
        {
            SDL_Log(
                "Failed to render player count field: %s",
                SDL_GetError()
            );
        }

        if (!app.playerCountf)
        {
            return;
        }

        if (!SDL_RenderTexture(
                app.renderer,
                app.playerCountf,
                nullptr,
                &app.playerCountButton.rect))
        {
            SDL_Log(
                "Failed to render player count button: %s",
                SDL_GetError()
            );
        }

        if (!app.playerCountTxt)
        {
            return;
        }

        float textW = 0.0f;
        float textH = 0.0f;

        if (!SDL_GetTextureSize(
                app.playerCountTxt,
                &textW,
                &textH))
        {
            SDL_Log(
                "SDL_GetTextureSize failed: %s",
                SDL_GetError()
            );

            return;
        }

        SDL_FRect textRect = {
            app.playerCountButton.rect.x +
                (app.playerCountButton.rect.w -
                 textW) / 2.0f,

            app.playerCountButton.rect.y +
                (app.playerCountButton.rect.h -
                 textH) / 2.0f,

            textW,
            textH
        };

        if (!SDL_RenderTexture(
                app.renderer,
                app.playerCountTxt,
                nullptr,
                &textRect))
        {
            SDL_Log(
                "Failed to render player count text: %s",
                SDL_GetError()
            );
        }
    }

    void renderNavigationButtons(
        Application& app)
    {
        if (!app.exitB || !app.nextB)
        {
            return;
        }

        if (!SDL_RenderTexture(
                app.renderer,
                app.exitB,
                nullptr,
                &app.exitButton.rect))
        {
            SDL_Log(
                "Failed to render exit button: %s",
                SDL_GetError()
            );
        }

        if (!SDL_RenderTexture(
                app.renderer,
                app.nextB,
                nullptr,
                &app.nextButton.rect))
        {
            SDL_Log(
                "Failed to render next button: %s",
                SDL_GetError()
            );
        }
    }

    bool renderInformation(
        Application& app)
    {
        if (!app.renderer ||
            !app.information)
        {
            return false;
        }

        if (!app.renderFullscreenTexture(
                app.information))
        {
            SDL_Log(
                "Failed to render information panel."
            );

            return false;
        }

        if (!app.infoClose)
        {
            return false;
        }

        if (!SDL_RenderTexture(
                app.renderer,
                app.infoClose,
                nullptr,
                &app.infoCloseButton.rect))
        {
            SDL_Log(
                "Failed to render information close button: %s",
                SDL_GetError()
            );

            return false;
        }

        return true;
    }
}

void Window::loop()
{
    Application app;
    GameLog game;

    if (!app.initialize())
    {
        return;
    }

    if (!app.bGAudio.audioPlayLoop(
            app.bGAudio.appBgSound))
    {
        app.cleanup();
        return;
    }

    while (running)
    {
        SDL_Event event{};

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                {
                    running = false;
                    break;
                }

                case SDL_EVENT_WINDOW_RESIZED:
                {
                    break;
                }

                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                {
                    if (event.button.button ==
                        SDL_BUTTON_LEFT)
                    {
                        const SDL_Event rawEvent =
                            event;

                        if (informationVisible)
                        {
                            handleInfo(
                                app,
                                *this,
                                rawEvent
                            );

                            break;
                        }

                        handleMute(
                            app,
                            audioMuted,
                            rawEvent
                        );

                        if (running)
                        {
                            handleInfo(
                                app,
                                *this,
                                rawEvent
                            );
                        }

                        if (running &&
                            !informationVisible)
                        {
                            handleLogicalPointerEvent(
                                app,
                                game,
                                *this,
                                event
                            );
                        }
                    }

                    break;
                }

                case SDL_EVENT_FINGER_DOWN:
                {
                    const SDL_Event rawEvent =
                        event;

                    if (informationVisible)
                    {
                        handleInfo(
                            app,
                            *this,
                            rawEvent
                        );

                        break;
                    }

                    handleMute(
                        app,
                        audioMuted,
                        rawEvent
                    );

                    if (running)
                    {
                        handleInfo(
                            app,
                            *this,
                            rawEvent
                        );
                    }

                    if (running &&
                        !informationVisible)
                    {
                        handleLogicalPointerEvent(
                            app,
                            game,
                            *this,
                            event
                        );
                    }

                    break;
                }

                case SDL_EVENT_TEXT_INPUT:
                {
                    if (app.playerCountActive &&
                        !informationVisible)
                    {
                        for (
                            const char* p =
                                event.text.text;
                            *p;
                            ++p)
                        {
                            if (*p >= '0' &&
                                *p <= '9')
                            {
                                app.playerCountText +=
                                    *p;
                            }
                        }

                        isDisplayError = false;

                        app.updatePlayerCountTexture();
                    }

                    break;
                }

                case SDL_EVENT_KEY_DOWN:
                {
                    if (app.playerCountActive &&
                        !informationVisible)
                    {
                        if (event.key.key ==
                                SDLK_BACKSPACE &&
                            !app.playerCountText.empty())
                        {
                            app.playerCountText.pop_back();

                            app.updatePlayerCountTexture();
                        }
                    }

                    break;
                }

                default:
                {
                    break;
                }
            }
        }

        SDL_SetRenderDrawColor(
            app.renderer,
            0,
            0,
            0,
            255
        );

        SDL_RenderClear(
            app.renderer
        );

        app.renderBackground();

        if (!informationVisible)
        {
            renderMuteButton(
                app,
                audioMuted
            );

            renderInfoButton(
                app
            );

            if (isDisplay)
            {
                renderPlayerCountScreen(
                    app
                );
            }

            if (gameState ==
                GameState::ShowPlayerMessage)
            {
                app.renderFullscreenTexture(
                    app.roleMessage
                );
            }

            if (gameState ==
                GameState::ShowRole)
            {
                if (currentPlayer >= 0 &&
                    currentPlayer <
                        static_cast<int>(
                            roles.size()))
                {
                    if (roles[currentPlayer] ==
                        "Mafia")
                    {
                        app.renderFullscreenTexture(
                            app.mafiaCard
                        );
                    }
                    else
                    {
                        app.renderFullscreenTexture(
                            app.civilianCard
                        );
                    }
                }
            }

            if (isDisplayError)
            {
                app.renderFullscreenTexture(
                    app.countError
                );
            }

            if (isDisplayButton)
            {
                renderNavigationButtons(
                    app
                );
            }
        }

        if (informationVisible)
        {
            renderInformation(
                app
            );
        }

        SDL_RenderPresent(
            app.renderer
        );
    }

    app.cleanup();
}