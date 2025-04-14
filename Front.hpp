#pragma once

#include <iostream>
#include <SDL.h>
#include <vector>
#include "Colors.hpp"

typedef struct MouseState {
    bool ButtonStates[5];
    int x, y;
} MouseState;

class InputManager
{
    /*
    The purpose of this class is to manage inputs of the game.
    */

public:
    InputManager();
    ~InputManager();

    bool HandleInputs();
    void InitMouseState();

    bool getKeyState(SDL_Scancode keycode) const { return this->inputKeyStates[keycode]; }
    bool getKeyPress(SDL_Scancode keycode) const { return (std::count(this->inputKeyPresses.begin(), this->inputKeyPresses.end(), keycode) != 0); }

    bool getMouseButtonState(int button) const { return this->mouseState->ButtonStates[button]; }
    bool getMouseButtonPress(int button) const { return (std::count(this->mouseButtonPresses.begin(), this->mouseButtonPresses.end(), button) != 0); }
    int getMouseX() const { return this->mouseState->x; }
    int getMouseY() const { return this->mouseState->y; }

private:
    MouseState* mouseState;
    std::vector<int> mouseButtonPresses; // SDL_BUTTON_LEFT, SDL_BUTTON_RIGHT, etc.
    SDL_Event* inputEvent;
    bool inputKeyStates[286]; // true = key held down, false = key not held down
    std::vector<SDL_Scancode> inputKeyPresses;
};


class FrontendManager
{
    /*
    The purpose of this class is to manage the frontend of the game.
    */

public:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Rect winRect;

    FrontendManager(int screenW, int screenH, int fps, const std::string windowTitle);
    ~FrontendManager();

    void PresentRenderer() const;
    void PauseDelay() const;

    int getScreenW() const { return this->screenW; }
    int getScreenH() const { return this->screenH; }
    int getScreenX() const { return this->screenX; }
    int getScreenY() const { return this->screenY; }

private:
    int screenW, screenH;
    int& screenX = screenW; // Alias to screenW
    int& screenY = screenH; // Alias to screenH
    int fps = 60;
    int framePauseDelay = 1000 / fps;
    std::string windowTitle;
};