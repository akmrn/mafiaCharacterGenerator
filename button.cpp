#include "button.h"

bool Button::isMouseInside(float x, float y) const
{
    return x >= rect.x &&
           x <= rect.x + rect.w &&
           y >= rect.y &&
           y <= rect.y + rect.h;
}