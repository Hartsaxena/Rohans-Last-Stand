#include "Menu.hpp"

#include "Front.hpp"
#include "Render.hpp"


MainMenu::MainMenu(Canvas* canvas, FrontendManager* frontend, InputManager* inputter)
    : frontend(frontend), // Initialize FrontendManager
    canvas(canvas),      // Initialize Canvas
    inputter(inputter)   // Initialize InputManager
{
    // Initialize remaining members
    font = new Font("Middle-Earth.ttf", 38);

    int xDimension = frontend->getScreenX();
    int yDimension = frontend->getScreenY();

    titleBox = new TextBox(xDimension / 2 - (450 / 2), yDimension / 2 - 300 - 50, 450, 100,
        "Rohan's Last Stand", font, OFFWHITE, BLACK, BLACK);

    playButton = new RenderableButton(xDimension / 2 - 100, yDimension / 2 + 100, 200, 50,
        GRAY, BLACK);
    playButton->addText("Play", font);

    exitButton = new RenderableButton(xDimension / 2 - 75, yDimension / 2 + 175, 150, 50,
        GRAY, MEDIUM_RED);
    exitButton->addText("Exit", font);
}

MainMenu::~MainMenu() {
    delete font;
    delete playButton;
    delete exitButton;
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
    canvas->blankScreen();
    canvas->drawTextBox(titleBox);
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