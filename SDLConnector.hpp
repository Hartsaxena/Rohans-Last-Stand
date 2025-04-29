#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <string>

#include "Render.hpp"
#include "Front.hpp"
#include "Game.hpp"
#include "Enemy.hpp"
#include "Menu.hpp"
#include "Colors.hpp"

enum Scene {
    MAIN_MENU,
    GAME,
    GAME_OVER,
    SETTINGS
};


class CardGraphic : public Button {
public:
    static const int cardW = 180, cardH = 250;
    static const int borderSize = 5;
    bool selected = false;

    CardGraphic(Card* card, int x, int y, Color color = CLEAR)
        : Button(x, y, cardW, cardH, color),
        card(card) {
    }

    void render(Canvas* canvas);
    void renderCardStats(Canvas* canvas);
    void setCard(Card* card) { this->card = card; }
    const Card* getCard() const { return this->card; }

    static std::map<SpecialAbility, std::string> specialAbilityNames;
    static std::map<PlayCondition, std::string> playConditionNames;
    static std::map<CardID, SDL_Texture*> cardTextures;
    static void loadTextures(SDL_Renderer* renderer);

private:
    const Card* card;
};

class CardSlot : public Button {
public:
    bool hovered = false;

    CardSlot(int x, int y, Color defaultColor = CLEAR, Color hoverColor = GHOST)
        : Button(x, y, CardGraphic::cardW, CardGraphic::cardH, defaultColor),
        defaultColor(defaultColor),
        hoverColor(hoverColor)
    {
    }
    void render(Canvas* canvas);

    bool addCardGraphic(CardGraphic* cardGraphic) {
        if (cardGraphic == nullptr) {
            std::cerr << "CardSlot: No card graphic to add\n";
            return false;
        }
        if (this->graphic != nullptr) {
            std::cerr << "CardSlot: Card graphic already exists\n";
            return false;
        }
        this->graphic = cardGraphic;
        return true;
    }

    bool removeCardGraphic() {
        if (this->graphic == nullptr) {
            std::cerr << "CardSlot: No card graphic to remove\n";
            return false;
        }
        this->graphic = nullptr;
        return true;
    }

private:
    CardGraphic* graphic = nullptr;
    Color defaultColor;
    Color hoverColor;
};


class SDLConnector {

public:
    SDLConnector(int xDimension, int yDimension, int fps, std::string windowTitle);
    ~SDLConnector();

    bool tick();

private:

    FrontendManager frontend;
    Canvas canvas;
    InputManager inputter;
    SDL_Texture* backgroundTexture;
    Font font;
    Scene scene = MAIN_MENU;
    MainMenu menu;

    // Game flow members
    bool gameStateChange = false;
    bool assaultReady = false;
    bool gameOver = false;

    // UI members
    CardGraphic* previewCardGraphic = nullptr;
    RenderableButton* lockButton;
    RenderableButton* assaultButton;
    TextBox* playerHealthCounter;
    TextBox* enemyHealthCounter;
    TextBox* turnTypeTextBox;

    // Helper members
    int& xDimension = canvas.xDimension;
    int& yDimension = canvas.yDimension;

    // Game-specific members
    Director game;
    EnemyAI enemy;
    std::vector<CardGraphic> cardGraphics;
    std::vector<CardSlot> assaultSlots;
    int selectedCardIndex = -1;
    int selectedSlotIndex = -1;

    bool menuTick();
    bool gameTick();
    void processClick(int mx, int my);
    void processRightClick(int mx, int my);
    void processMouseMove(int mx, int my);
    bool loadBackgroundTexture();
    void freeCardTextures();
    void renderBackground();
    void renderBoard();
    void renderUI();
    void resetGraphics();
};
