#pragma once

#include <vector>
#include <string>

enum class GameState
{
    PlayerCount,
    ShowPlayerMessage,
    ShowRole,
    Finished
};

struct Window
{
    bool running = true;
    bool isDisplay = true;
    bool isDisplayButton = true;
    bool isDisplayError = false;
    bool audioMuted = false;
    bool informationVisible = false;

    GameState gameState = GameState::PlayerCount;

    int playerCount = 0;
    int currentPlayer = 0;

    std::vector<std::string> roles;

    void loop();
};