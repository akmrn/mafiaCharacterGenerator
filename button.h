#pragma once

#include <SDL3/SDL.h>

struct Button
{
    SDL_FRect rect;

    bool isMouseInside(float x, float y) const;
};