#include "Menu.hpp"
#include "SDL_image.h"

#include "Front.hpp"
#include "Render.hpp"


MainMenu::MainMenu(Canvas* canvas, FrontendManager* frontend, InputManager* inputter)
    : frontend(frontend), // Initialize FrontendManager
    canvas(canvas),      // Initialize Canvas
    inputter(inputter)   // Initialize InputManager
{
    // Initialize remaining members
    font = new Font("Middle-Earth.ttf", 38);
    smallFont = new Font("Middle-Earth.ttf", 25);

    int xDimension = frontend->getScreenX();
    int yDimension = frontend->getScreenY();

    // Load background texture
    SDL_Surface* backgroundSurface = IMG_Load("menubg.png");
    if (backgroundSurface == nullptr) {
        std::cerr << "Failed to load background image: " << IMG_GetError() << "\n";
        return;
    }
    backgroundTexture = SDL_CreateTextureFromSurface(canvas->renderer, backgroundSurface);
    if (backgroundTexture == nullptr) {
        std::cerr << "Failed to create background texture: " << SDL_GetError() << "\n";
        return;
    }
    SDL_FreeSurface(backgroundSurface); // Free the surface after creating the texture

    titleBox = new TextBox(xDimension / 2 - (450 / 2), yDimension / 2 - 300 - 50, 450, 100,
        "Rohan's Last Stand", font, BROWN, GRAY, BLACK);

    versionBox = new TextBox(xDimension / 2 - (300 / 2), yDimension / 2 - 250, 300, 50,
        "v1.0.0", smallFont, CLEAR, CLEAR, OFFWHITE);

    playButton = new RenderableButton(xDimension / 2 - 175, yDimension / 2 + 120, 200, 60,
        LIGHT_BROWN, BLACK);
    playButton->addText("Play", font);

    exitButton = new RenderableButton(xDimension - 260, yDimension / 2 + 135, 150, 60,
        GRAY, MEDIUM_RED);
    exitButton->addText("Exit", font);
}

MainMenu::~MainMenu() {
    delete font;
    delete smallFont;

    delete titleBox;
    delete versionBox;
    delete playButton;
    delete exitButton;
    SDL_DestroyTexture(backgroundTexture);
}

bool MainMenu::tick() {
    // Handle events

    bool isRunning = inputter->HandleInputs();

    if (inputter->getMouseButtonPress(SDL_BUTTON_LEFT)) {
        isRunning = processLeftClick(inputter->getMouseX(), inputter->getMouseY());
    }
    if (inputter->getMouseMove()) {
        processMouseMove(inputter->getMouseX(), inputter->getMouseY());
    }

    // Render the menu
    canvas->fillScreenColor(DARK_BROWN);

    SDL_Rect destRect = {
        0, 0, frontend->getScreenX(), frontend->getScreenY()
    };
    SDL_RenderCopy(canvas->renderer, backgroundTexture, nullptr, &destRect);

    canvas->drawTextBox(titleBox);
    canvas->drawTextBox(versionBox);
    playButton->render(canvas);
    exitButton->render(canvas);

    frontend->PresentRenderer();
    frontend->PauseDelay();

    return isRunning;
}

bool MainMenu::processLeftClick(int mx, int my) {

    bool isRunning = true;

    if (playButton->collision(mx, my)) {
        std::cout << "Play button clicked\n";
        play = true;
    }
    else if (exitButton->collision(mx, my)) {
        std::cout << "Exit button clicked\n";
        isRunning = false; // Exit the game
    }

    return isRunning;
}

void MainMenu::processMouseMove(int mx, int my) {
    playButton->hovered = playButton->collision(mx, my);
    exitButton->hovered = exitButton->collision(mx, my);
}