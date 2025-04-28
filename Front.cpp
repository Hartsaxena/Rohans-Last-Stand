/*
This file contains methods that manage the SDL2 library, which is used as the frontend of the game. This includes window creation, rendering, and event handling.
*/
#include <iostream>
#include <windows.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "Front.hpp"


FrontendManager::FrontendManager(int screenW, int screenH, int fps, const std::string windowTitle)
{
    /*
    This function initializes the SDL2 library. It creates a window and renderer, and returns 0 if successful, or -1 if not.
    */

    this->screenW = screenW;
    this->screenH = screenH;
    this->fps = fps;
    this->windowTitle = windowTitle;

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"); // Linear filtering

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { // Initialize SDL
        std::cout << "Failed to initialize SDL.\n";
        std::cout << SDL_GetError();
        SDL_Quit();
        exit(1);
    }

    SDL_Delay(100); // Delay to allow SDL to initialize properly, just in case.

    Uint32 windowFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
    this->window = SDL_CreateWindow(windowTitle.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenW, screenH,
        windowFlags);
    if (this->window == NULL) {
        std::cout << "FATAL ERROR: Window could not be displayed.\n";
        std::cout << SDL_GetError();
        SDL_Quit();
        exit(1); // Quit the program
    }

    // Everything worked well so far, so let's create the renderer
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
    this->winRect = { 0, 0, screenW, screenH };

    // Some settings for renderer
    SDL_SetRenderDrawBlendMode(this->renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderSetLogicalSize(renderer, screenW, screenH);
    SDL_RenderSetIntegerScale(renderer, SDL_TRUE);

    // Set up TTF and text rendering
    TTF_Init();

    // Set up SDL_image for loading images
    IMG_Init(IMG_INIT_PNG); // Using PNG format for images
}


FrontendManager::~FrontendManager()
{
    /*
    Destructor for the FrontendManager class.
    Quits SDL2 and frees memory.
    */
    if (this->window != NULL) {
        SDL_DestroyWindow(this->window);
        SDL_DestroyRenderer(this->renderer);
    }
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void FrontendManager::PresentRenderer() const
{
    /*
    This function presents the renderer to the window.
    */
    SDL_RenderPresent(this->renderer);
}

void FrontendManager::PauseDelay() const
{
    /*
    This function pauses the game for a short time, to limit the frame rate.
    */
    SDL_Delay(this->framePauseDelay);
}

void FrontendManager::ToggleFullscreen() const
{
    /*
    This function toggles fullscreen mode on and off.
    */
    Uint32 flags = SDL_GetWindowFlags(this->window);
    if (flags & SDL_WINDOW_FULLSCREEN) {
        SDL_SetWindowFullscreen(this->window, 0);
    }
    else {
        SDL_SetWindowFullscreen(this->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
}


InputManager::InputManager()
{
    /*
    Constructor for the InputManager class.
    */

    for (int i = 0; i < 286; i++) {
        this->inputKeyStates[i] = false;
    }

    this->inputEvent = new SDL_Event;
    if (this->inputEvent == NULL) {
        std::cout << "Failed to allocate memory for input event data.\n";
        exit(1);
    }

    this->InitMouseState();
}


InputManager::~InputManager()
{
    /*
    Destructor for the InputManager class.
    Frees memory.
    */
    delete this->inputEvent;
    delete this->mouseState;
}


bool InputManager::HandleInputs()
{
    /*
    This function handles all input events. It returns true if the game should continue running, false if it should quit.
    */
    bool IsRunning = true;
    inputKeyPresses.clear();
    mouseButtonPresses.clear();
    this->mouseMovement = false;

    while (SDL_PollEvent(this->inputEvent) > 0) {

        SDL_Scancode scancode = inputEvent->key.keysym.scancode;
        switch (inputEvent->type) {

        case SDL_KEYDOWN: { // Key presses
            this->inputKeyStates[scancode] = true;
            this->inputKeyPresses.push_back(scancode);
            break;
        }
        case SDL_KEYUP: {
            this->inputKeyStates[scancode] = false;
            break;
        }

        case SDL_MOUSEBUTTONDOWN: { // Mouse movement / presses
            this->mouseState->ButtonStates[inputEvent->button.button] = true;
            this->mouseButtonPresses.push_back(inputEvent->button.button);
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            this->mouseState->ButtonStates[inputEvent->button.button] = false;
            break;
        }
        case SDL_MOUSEMOTION: {
            this->mouseState->x = inputEvent->motion.x;
            this->mouseState->y = inputEvent->motion.y;
            this->mouseMovement = true;
            break;
        }

                            // Exiting game
        case SDL_QUIT: {
            IsRunning = false;
            break;
        }
        default: {
            break;
        }
        }
    }
    return IsRunning;
}

void InputManager::InitMouseState() {
    MouseState* currentMouseState = new MouseState;
    if (currentMouseState == NULL) {
        std::cout << "Failed to allocate memory for Mouse State data.\n";
        exit(1);
    }

    for (int i = 0; i < 5; i++)
        currentMouseState->ButtonStates[i] = false;
    currentMouseState->x = 0;
    currentMouseState->y = 0;

    this->mouseState = currentMouseState;
}