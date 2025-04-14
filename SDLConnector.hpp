#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "Render.hpp"
#include "Front.hpp"
#include "Game.hpp"


class SDLConnector {

public:
    SDLConnector(int xDimension, int yDimension, int fps, std::string windowTitle);
    ~SDLConnector() = default;
    
    bool tick();

private:
    FrontendManager frontend;
    Canvas canvas;
    InputManager inputter;
    Font font;

    // Helper members

    int& xDimension = canvas.xDimension;
    int& yDimension = canvas.yDimension;

    // Game-specific members

    Director game;
};