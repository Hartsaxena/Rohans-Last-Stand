#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "SDLConnector.hpp"
#include "Front.hpp"
#include "Render.hpp"
#include "Game.hpp"


SDLConnector::SDLConnector(int xDimension, int yDimension, int fps, const std::string windowTitle) 
   : frontend(xDimension, yDimension, fps, windowTitle), // Use initializer list to construct FrontendManager
     canvas(frontend.renderer),                          // Initialize Canvas with the renderer
     inputter(),                                         // Initialize InputManager
     font("Middle-Earth.ttf", 25)                        // Initialize Font
{
}

bool SDLConnector::tick() {
    // Handle Events and update keyboard
    bool isRunning = inputter.HandleInputs();

    if (inputter.getMouseButtonPress(SDL_BUTTON_LEFT)) {
        std::cout << "Left Mouse clicked at: " << inputter.getMouseX() << ", " << inputter.getMouseY() << std::endl;
    }

    canvas.blankScreen();
    canvas.renderText("Hello, World!", &font, 100, 100, WHITE);
    frontend.PresentRenderer();
    frontend.PauseDelay();

    return isRunning;
}