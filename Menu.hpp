#pragma once

#include "Render.hpp"
#include "Front.hpp"

class MainMenu {
public:
    MainMenu(Canvas* canvas, FrontendManager* frontend, InputManager* inputter);
    ~MainMenu();

    bool tick();

    bool isPlay() const { return play; }

private:
    FrontendManager* frontend;
    InputManager* inputter;
    Font* font;
    Font* smallFont;
    Canvas* canvas;
    SDL_Texture* backgroundTexture;
    TextBox* titleBox;
    TextBox* versionBox;
    RenderableButton* playButton;
    RenderableButton* exitButton;

    bool play = false;

    bool processLeftClick(int mx, int my);
    void processMouseMove(int mx, int my);
};