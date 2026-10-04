#include "gameLogic.h"

bool GameLog::playerCountRule(int playerCount)
{
    if (playerCount < 3 || playerCount > 50)
    {
        return false;
    }
    
    return true;
}

std::vector<std::string> GameLog::generateRoles(int playerCount)
{
    std::vector<std::string> roles;
    int mafiaCount = 0;

    if (playerCount <= 5)
        mafiaCount = 1;
    else if (playerCount <= 7)
        mafiaCount = 2;
    else if (playerCount <= 10)
        mafiaCount = 3;
    else
        mafiaCount = playerCount / 3;

    for (int i = 0; i < mafiaCount; i++)
    {
        roles.push_back("Mafia");
    }

    int citizenCount = playerCount - mafiaCount;

    for (int i = 0; i < citizenCount; i++)
    {
        roles.push_back("Citizen");
    }

    std::random_device rd;
    std::mt19937 generator(rd());

    std::shuffle(
        roles.begin(),
        roles.end(),
        generator
    );

    return roles;
}

void GameLog::roleGiver(std::vector<std::string> roles, int playerCount)
{
    std::random_device rd;
    std::mt19937 generator(rd());

    std::shuffle(roles.begin(), roles.end(), generator);

    for (int i = 0; i < playerCount; i++)
    {
        std::cout << "Player " << i + 1 << ": "
                  << roles[i] << std::endl;
    }
}