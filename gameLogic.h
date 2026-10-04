#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

struct GameLog
{
    bool playerCountRule(int playerCount);
    std::vector<std::string> generateRoles(int playerCount);
    void roleGiver(std::vector<std::string> roles, int playerCount);
};