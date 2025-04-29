#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <stdexcept>

#include "SDLConnector.hpp"
#include "Front.hpp"
#include "Render.hpp"
#include "Game.hpp"


void RenderableButton::render(Canvas* canvas) {
    if (hovered) {
        this->setColor(hoveredColor);
    }
    else {
        this->setColor(defaultColor);
    }

    int borderSize = 2;
    canvas->drawRect(x - borderSize, y - borderSize, w + (2 * borderSize), h + (2 * borderSize), borderColor);
    canvas->drawRect(this);

    if (font != nullptr) {
        canvas->renderTextCenter(
            text, font,
            x + w / 2, y + h / 2,
            hovered ? textColor : textHoverColor
        );
    }
}

void CardGraphic::render(Canvas* canvas) {
    if (card == nullptr) {
        std::cerr << "CardGraphic: No card to render\n";
        return;
    }

    // Draw Border
    Color borderColor = (selected) ? BLUE : BLACK;
    Rectangle borderRect(
        x - borderSize,
        y - borderSize,
        w + 2 * borderSize,
        h + 2 * borderSize,
        borderColor
    );
    canvas->drawRect(&borderRect);

    CardID id = card->getType().id;
    SDL_Texture* texture = cardTextures[id];
    if (texture == nullptr) {
        std::cerr << "CardGraphic: No texture for card ID " << id << "\n";
        return;
    }

    SDL_Rect destRect = rect;
    SDL_RenderCopy(canvas->renderer, texture, nullptr, &destRect);

    // Render the card text
    renderCardStats(canvas);
}


std::map<SpecialAbility, std::string> CardGraphic::specialAbilityNames = {
    {NONE, "None"},
    {SURPRISE, "Surprise"},
    {INSPIRE, "Inspire"},
    {REINFORCE, "Reinforce"},
    {RALLY, "Rally"},
    {HATE, "Hate"},
    {ARMOR_PIERCE, "Armor Pierce"},
    {KAMIKAZE, "Kamikaze"}
};
std::map<PlayCondition, std::string> CardGraphic::playConditionNames = {
    {FREE, "Free"},
    {ATTACK_ONLY, "Aggressive"},
    {DEFENSE_ONLY, "Defensive"}
};
void CardGraphic::renderCardStats(Canvas* canvas) {
    static Font font("Middle-Earth.ttf", 20);      // For name
    static Font fontSmall("Middle-Earth.ttf", 15); // For stats + special ability

    if (card == nullptr) {
        std::cerr << "CardGraphic: No card to render text for\n";
        return;
    }

    const int cx = rect.x + rect.w / 2;

    // Attack (top left)
    int ax = rect.x + 10;
    int ay = rect.y + 10;
    Color col = (card->attack > card->getType().attack) ? GREEN : GRAY;
    canvas->renderText("ATK: " + std::to_string(card->attack), &fontSmall, ax, ay, col);

    // Defense (top right)
    std::string txt = "DEF: " + std::to_string(card->defense);
    int tw = 0, th = 0;
    TTF_SizeText(fontSmall.getFont(), txt.c_str(), &tw, &th);
    int dx = rect.x + rect.w - tw - 10;
    int dy = rect.y + 10;
    canvas->renderText(txt, &fontSmall, dx, dy, GRAY);

    // Special Ability (~2/3 down)
    if (card->getType().special != NONE) {
        int sy = rect.y + (2 * rect.h / 3);
        canvas->renderTextCenter(
            specialAbilityNames.at(card->getType().special),
            &fontSmall, cx, sy, GRAY
        );
    }

    // Play Condition (Just Above HP at Bottom)
    if (card->getType().condition != FREE) {
        int pcY = ((rect.y + rect.h - 50) + (rect.y + (2 * rect.h / 3))) / 2;
        canvas->renderTextCenter(
            playConditionNames.at(card->getType().condition),
            &fontSmall, cx, pcY, GRAY);
    }

    // Name (Just Above HP at Bottom)
    int healthY = rect.y + rect.h - 25;
    int nameY = healthY - 20;
    canvas->renderTextCenter(
        card->getType().name,
        &font, cx, nameY, GRAY
    );

    // Health (Bottom Center)
    std::string ht = "HP: " + std::to_string(card->currHealth);
    Color hc = (card->currHealth == card->getType().maxHealth) ? GREEN : RED;
    int hy = rect.y + rect.h - 20;
    canvas->renderTextCenter(ht, &fontSmall, cx, hy, hc);
}


std::map<CardID, SDL_Texture*> CardGraphic::cardTextures; // Declaring a storage for textures so the compiler doesn't freak out
void CardGraphic::loadTextures(SDL_Renderer* renderer) {
    const std::map<CardID, CardType>& registry = Board::getCardRegistry();

    // Create a default gray texture
    const int defaultW = 400, defaultH = 400;
    SDL_Surface* defaultSurface = SDL_CreateRGBSurface(0, defaultW, defaultH, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    if (!defaultSurface) {
        std::cerr << "Failed to create default surface: " << SDL_GetError() << "\n";
        return;
    }

    // Fill it with gray (150, 150, 150)
    SDL_FillRect(defaultSurface, nullptr, SDL_MapRGB(defaultSurface->format, 150, 150, 150));
    SDL_Texture* defaultTexture = SDL_CreateTextureFromSurface(renderer, defaultSurface);
    SDL_FreeSurface(defaultSurface);
    if (!defaultTexture) {
        std::cerr << "Failed to create default texture: " << SDL_GetError() << "\n";
        return;
    }

    // Store it under BLANK key
    cardTextures[BLANK] = defaultTexture;

    // Actually load the textures for each card
    for (const auto& pair : registry) {
        CardID id = pair.first;
        std::string name = pair.second.name;
        std::string path = "cards/" + name + ".png"; // e.g. Strider -> cards/Strider.png

        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            std::cerr << "Failed to load image " << path << ": " << IMG_GetError() << "\n";
            cardTextures[id] = defaultTexture;
            continue;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if (!texture) {
            std::cerr << "Failed to create texture for " << path << ": " << SDL_GetError() << "\n";
            cardTextures[id] = defaultTexture;
            continue;
        }

        cardTextures[id] = texture;
    }
}

void CardSlot::render(Canvas* canvas) {

    if (graphic != nullptr) {
        graphic->render(canvas);
        return;
    }

    if (hovered) {
        this->color = hoverColor;
    }
    else {
        this->color = defaultColor;
    }
    canvas->drawRect(this);
}

SDLConnector::SDLConnector(int xDimension, int yDimension, int fps, const std::string windowTitle)
    : frontend(xDimension, yDimension, fps, windowTitle), // Use initializer list to construct FrontendManager
    canvas(frontend.renderer),                   // Initialize Canvas with the renderer
    inputter(),                                  // Initialize InputManager
    font("Middle-Earth.ttf", 25),                // Initialize Font
    game(),                                      // Initialize Director
    enemy(&game),                                // Initialize EnemyAI
    menu(&canvas, &frontend, &inputter)         // Initialize MainMenu
{
    CardGraphic::loadTextures(frontend.renderer); // Load card textures
    if (!loadBackgroundTexture()) {
        std::cerr << "Failed to load background texture\n";
        throw std::runtime_error("Failed to load background texture");
    }

    lockButton = new RenderableButton(xDimension - 200, yDimension / 2 - 40, 180, 80);
    lockButton->addText("Finish", &font);
    assaultButton = new RenderableButton(xDimension - 340, yDimension / 2 - 25, 120, 50,
        OFFWHITE, MEDIUM_RED.alpha(200));
    assaultButton->addText("Fight!", &font);
    assaultButton->changeTextColor(BLACK);
    assaultButton->changeTextHoverColor(MEDIUM_RED);

    playerHealthCounter = new TextBox(50, yDimension - 200, 150, 80,
        "HP: " + std::to_string(game.getPlayerHealth()), &font,
        MEDIUM_GREEN);
    enemyHealthCounter = new TextBox(50, 100, 150, 80,
        "HP: " + std::to_string(game.getEnemyHealth()), &font,
        MEDIUM_GREEN);

    turnTypeTextBox = new TextBox(xDimension - 200 - 50, yDimension - 200, 200, 80,
        "Attacking", &font,
        GRAY);

    resetGraphics(); // Reset cardGraphics
}

SDLConnector::~SDLConnector() {
    // Free any loaded card textures
    freeCardTextures();

    // Free background
    if (backgroundTexture) {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = nullptr;
    }

    delete lockButton;
    delete assaultButton;
    delete playerHealthCounter;
    delete enemyHealthCounter;
    delete turnTypeTextBox;

    // Tear down SDL
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


bool SDLConnector::tick() {
    switch (scene) {
    case MAIN_MENU:
        return menuTick();
    case GAME:
        return gameTick();
    default:
        throw std::out_of_range("Invalid scene");
        return false;
    }
}

bool SDLConnector::menuTick() {
    bool isRunning = menu.tick();
    if (menu.isPlay()) {
        std::cout << "Starting Game\n";
        scene = GAME;
    }

    return isRunning;
}

bool SDLConnector::gameTick() {

    // Handle Events and update keyboard
    bool isRunning = inputter.HandleInputs();

    int mx = inputter.getMouseX(), my = inputter.getMouseY();
    if (inputter.getMouseButtonPress(SDL_BUTTON_LEFT)) {
        std::cout << "Left Mouse clicked at: " << mx << ", " << my << "\n";
        processClick(mx, my);
    }
    if (inputter.getMouseButtonPress(SDL_BUTTON_RIGHT)) {
        std::cout << "Right Mouse clicked at: " << mx << ", " << my << "\n";
        processRightClick(mx, my);
    }
    if (inputter.getMouseMove()) {
        processMouseMove(mx, my);
    }

    if (inputter.getKeyPress(SDL_SCANCODE_H)) {
        std::cout << "H key pressed\n";
        frontend.ToggleFullscreen();
        std::cout << "Toggled fullscreen\n";
    }

    if (gameStateChange) {
        resetGraphics();
        game.printBoard();
        gameStateChange = false;
    }

    this->renderBackground();
    this->renderBoard();
    this->renderUI();
    frontend.PresentRenderer();
    frontend.PauseDelay();

    return isRunning;
}

void SDLConnector::processClick(int mx, int my) {

    if (gameOver) {
        return;
    }

    if (assaultReady) { // The player can only interact with the Assault button when it's available.

        if (assaultButton->hovered) {
            std::cout << "Assault button clicked\n";
            if (game.turnAttack()) {
                std::cout << "Assault phase completed\n";
                game.first = !game.first; // Switch turns
                game.drawCards(true); // Draw until 7 cards are in hand;
                if (!game.first) {
                    enemy.turn();
                }

                gameStateChange = true;
            }
            else {
                std::cout << "Game over\n";
                gameOver = true;
            }
            assaultReady = false;
            gameStateChange = true;
        }

        return;
    }

    if (lockButton->hovered) {
        std::cout << "Lock button clicked\n";
        if (game.first) {
            enemy.turn();
        }
        assaultReady = true;

        gameStateChange = true;
        return;
    }

    // Select/deselect *only* among your hand-card graphics
    int totalG = static_cast<int>(cardGraphics.size());
    int playerSz = static_cast<int>(game.getPlayerHand().size());
    int enemySz = static_cast<int>(game.getEnemyHand().size());
    int handStart = totalG - playerSz - enemySz;
    int handEnd = handStart + playerSz;

    for (int i = handStart; i < handEnd; i++) {
        if (!cardGraphics[i].collision(mx, my)) continue;

        // Deselect previous
        if (selectedCardIndex != -1 && selectedCardIndex != i) {
            cardGraphics[selectedCardIndex].selected = false;
        }

        // Toggle this one
        cardGraphics[i].selected = !cardGraphics[i].selected;
        if (cardGraphics[i].selected) {
            selectedCardIndex = i;
            std::cout << "Selected card: "
                << cardGraphics[i].getCard()->getType().name << "\n";
        }
        else {
            selectedCardIndex = -1;
        }
        return;
    }

    // If no card is selected, bail out
    if (selectedCardIndex < handStart || selectedCardIndex >= handEnd) {
        std::cout << "No card selected\n";
        return;
    }

    // Try to play it to one of the assault slots
    for (int i = 0; i < assaultSlots.size(); i++) {
        if (!assaultSlots[i].collision(mx, my)) continue;

        // even = your row, odd = enemy row
        if (i % 2 != 0) {
            std::cout << "Cannot play there\n";
            return;
        }

        int boardPos = i / 2;
        std::cout << "Playing to board slot " << boardPos << "\n";

        // map to 0…playerSz-1
        int handIndex = selectedCardIndex - handStart;

        if (game.playCard(true, handIndex, boardPos)) {
            std::cout << "Played card in slot " << boardPos << "\n";

            // remove from hand graphic
            cardGraphics[selectedCardIndex].setCard(nullptr);
            cardGraphics[selectedCardIndex].selected = false;

            assaultSlots[i].addCardGraphic(&cardGraphics[selectedCardIndex]);
            selectedCardIndex = -1;
            gameStateChange = true;
        }
        else {
            std::cout << "Failed to play card\n";
        }
        return;
    }
}



void SDLConnector::processRightClick(int mx, int my) { // TODO: COMPLETE

    if (gameOver) {
        return;
    }

    CardGraphic* previewCardGraphic = nullptr;
    for (int i = 0; i < cardGraphics.size(); i++) {
        if (cardGraphics[i].collision(mx, my)) {
            previewCardGraphic = &cardGraphics[i];
            break;
        }
    }

    if (previewCardGraphic == nullptr) {
        return;
    }
}

void SDLConnector::processMouseMove(int mx, int my) {

    if (assaultReady) {
        if (assaultButton->collision(mx, my)) {
            assaultButton->hovered = true;
        }
        else {
            assaultButton->hovered = false;
        }

        return;
    }

    for (auto& slot : assaultSlots) {

        if (slot.collision(mx, my)) {
            slot.hovered = true;
        }
        else
            slot.hovered = false;

    }

    if (lockButton->collision(mx, my)) {
        lockButton->hovered = true;
    }
    else {
        lockButton->hovered = false;
    }
}

bool SDLConnector::loadBackgroundTexture() {
    // Load the background texture
    SDL_Surface* surface = IMG_Load("bg.png");
    if (!surface) {
        std::cerr << "Failed to load background image: " << IMG_GetError() << "\n";
        return false;
    }
    backgroundTexture = SDL_CreateTextureFromSurface(frontend.renderer, surface);
    SDL_FreeSurface(surface);
    if (!backgroundTexture) {
        std::cerr << "Failed to create background texture: " << SDL_GetError() << "\n";
        return false;
    }
    return true;
}

void SDLConnector::freeCardTextures() {
    for (auto& p : CardGraphic::cardTextures) {
        if (p.second) {
            SDL_DestroyTexture(p.second);
            p.second = nullptr;
        }
    }
    CardGraphic::cardTextures.clear();
}

void SDLConnector::renderBackground() {
    SDL_Rect destRect = { 0, 0, xDimension, yDimension };
    SDL_RenderCopy(frontend.renderer, backgroundTexture, nullptr, &destRect);
}

void SDLConnector::renderBoard() {
    // Draw Cards
    for (CardGraphic& graphic : cardGraphics) {
        graphic.render(&canvas);
    }
    for (Button& slot : assaultSlots) {
        slot.render(&canvas);
    }
}

void SDLConnector::renderUI() {

    if (gameOver) {
        canvas.renderTextCenter("Game Over!", &font, xDimension / 2, yDimension / 2, MEDIUM_RED);
    }

    if (assaultReady) {
        assaultButton->render(&canvas);
    }
    lockButton->render(&canvas);
    canvas.drawTextBox(playerHealthCounter);
    canvas.drawTextBox(enemyHealthCounter);

    if (game.first) {
        turnTypeTextBox->setText("Attacking");
    }
    else {
        turnTypeTextBox->setText("Defending");
    }
    canvas.drawTextBox(turnTypeTextBox);
}

void SDLConnector::resetGraphics() {
    // Clear previous graphics and slots
    cardGraphics.clear();
    assaultSlots.clear();

    // Prevent vector reallocation (so pointers into cardGraphics stay valid)
    size_t boardSlots = game.getEnemyCards().size() * 2;
    assaultSlots.reserve(boardSlots);
    size_t maxGraphics = (game.getEnemyCards().size() + game.getPlayerCards().size())
        + game.getPlayerHand().size() + game.getEnemyHand().size();
    cardGraphics.reserve(maxGraphics);

    // Update health counters
    playerHealthCounter->setText("HP: " + std::to_string(game.getPlayerHealth()));
    enemyHealthCounter->setText("HP: " + std::to_string(game.getEnemyHealth()));

    if (game.getPlayerHealth() <= 10) {
        playerHealthCounter->color = RED;
    }
    else {
        playerHealthCounter->color = MEDIUM_GREEN;
    }
    if (game.getEnemyHealth() <= 10) {
        enemyHealthCounter->color = RED;
    }
    else {
        enemyHealthCounter->color = MEDIUM_GREEN;
    }

    // Layout constants
    const int cardW = CardGraphic::cardW;
    const int cardH = CardGraphic::cardH;
    const int cardSpacing = 15;
    const int cardBorder = CardGraphic::borderSize;
    const int middleSplit = 20;

    // Board data
    const auto& enemyCards = game.getEnemyCards();
    const auto& playerCards = game.getPlayerCards();

    // Compute assault area dimensions
    const int assaultW = (5 * (cardW + 2 * cardSpacing)) + (4 * cardSpacing);
    const int assaultH = 2 * (2 * cardBorder + cardH) + middleSplit;
    const int firstCardX = (xDimension - assaultW) / 2;
    const int enemyCardY = (yDimension - assaultH) / 2;
    const int playerCardY = (yDimension / 2) + (middleSplit / 2) + cardBorder;

    // Create slots and add any cards in play
    for (size_t i = 0; i < enemyCards.size(); ++i) {
        int x = firstCardX + static_cast<int>(i) * (cardW + cardSpacing);
        int ey = enemyCardY + cardBorder;
        int py = playerCardY + cardBorder;

        // Add two slots: player row then enemy row
        assaultSlots.push_back(CardSlot(x, py));   // index = 2*i
        assaultSlots.push_back(CardSlot(x, ey));   // index = 2*i + 1

        // AI card in slot?
        if (enemyCards[i] != nullptr) {
            cardGraphics.push_back(CardGraphic(enemyCards[i], x, ey));
            assaultSlots[2 * i + 1].addCardGraphic(&cardGraphics.back());
        }
        // Player card in slot?
        if (playerCards[i] != nullptr) {
            cardGraphics.push_back(CardGraphic(playerCards[i], x, py));
            assaultSlots[2 * i].addCardGraphic(&cardGraphics.back());
        }
    }

    // Draw the player's hand at the bottom
    const std::vector<Card*>& playerHand = game.getPlayerHand();
    const int vMargin = 20;
    const int handW = static_cast<int>(playerHand.size()) * (cardW + 2 * cardSpacing);
    const int handH = cardH + 2 * cardBorder;
    const int handX = (xDimension - handW) / 2;
    const int handY = yDimension - vMargin - handH;

    for (size_t i = 0; i < playerHand.size(); ++i) {
        int x = handX + static_cast<int>(i) * (cardW + cardSpacing);
        int y = handY + cardBorder;
        cardGraphics.push_back(CardGraphic(playerHand[i], x, y));
    }

    // Draw the enemy's hand (face-down) at the top
    const std::vector<Card*>& enemyHand = game.getEnemyHand();
    const int eHandY = vMargin + cardBorder;
    const int ehandW = static_cast<int>(enemyHand.size()) * (cardW + 2 * cardSpacing);
    const int ehandX = (xDimension - ehandW) / 2;

    for (size_t i = 0; i < enemyHand.size(); ++i) {
        if (enemyHand[i] == nullptr) continue;
        int x = ehandX + static_cast<int>(i) * (cardW + cardSpacing);
        int y = eHandY;
        cardGraphics.push_back(CardGraphic(enemyHand[i], x, y));
    }
}