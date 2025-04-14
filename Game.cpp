#include "Game.hpp"

#include <iostream>
#include <iomanip> // Used for the command prompt interface
#include <random>


/**
 * @brief The Card Registry is a static map that contains all the card types and their attributes.
 * The general format is:
 *
 * {CardID, {name, maxHealth, attack, defense}}
 */
std::map<CardID, CardType> Board::cardRegistry = {
    {ARAGORN, {"Strider", 7, 7, 2}},
    {ELVEN_SON, {"Elven Son", 4, 9, 0}},
    {RECRUIT, {"Recruit", 3, 2, 2}},

    {URUK, {"Uruk-Hai", 3, 4, 1}},
    {ORC, {"Orc", 2, 3, 0}},
    {BATTERING_RAM, {"Battering Ram", 2, 8, 1}},
};

Card::Card(CardType type) : type(type) {
    this->currHealth = type.maxHealth;
    this->attack = type.attack;
    this->defense = type.defense;
}

Board::Board() {
    this->resetAssault();
}

Board::~Board() {
    for (int i = 0; i < 5; i++) {
        if (this->enemyCards[i] != nullptr) {
            delete this->enemyCards[i];
        }
        if (this->playerCards[i] != nullptr) {
            delete this->playerCards[i];
        }
    }
}

void Board::resetAssault() {
    this->enemyCards.clear();
    this->playerCards.clear();

    for (int i = 0; i < 5; i++) {
        this->enemyCards.push_back(nullptr);
        this->playerCards.push_back(nullptr);
    }
}

void Board::print() const {
    std::cout << "Enemy Health: " << this->enemyHealth << "\n";
    std::cout << "Enemy Deck: " << this->enemyDeck.size() << " Cards\n";
    std::cout << "Enemy Hand:\n";
    for (Card* card : this->enemyHand) {
        std::cout << card->getType().name << "\n";
    }

    // Print the Assault. Format so that cards' opposing card are aligned.
    int slotWidth = 15;  // Adjust this to fit the widest card name. Default is 15 due to attributes
    for (int i = 0; i < 5; i++) {
        if (this->enemyCards[i] != nullptr) {
            slotWidth = std::max(slotWidth, static_cast<int>(this->enemyCards[i]->getType().name.length()));
        }
        if (this->playerCards[i] != nullptr) {
            slotWidth = std::max(slotWidth, static_cast<int>(this->playerCards[i]->getType().name.length()));
        }
    }
    slotWidth++; // Add 1 for the minimum space between cards

    // Enemy Cards
    std::cout << "\nEnemy Cards\n";
    for (int i = 0; i < 5; i++) {
        if (this->enemyCards[i] != nullptr) {
            std::cout << std::left << std::setw(slotWidth)
                << this->enemyCards[i]->getType().name;
        }
        else {
            std::cout << std::left << std::setw(slotWidth) << "Empty";
        }
    }
    std::cout << "\n";
    // Print attributes
    for (int i = 0; i < 5; i++) {
        std::cout << std::left << std::setw(slotWidth)
            << (this->enemyCards[i] != nullptr ? "Attack: " + std::to_string(this->enemyCards[i]->attack) : "~");
    }
    std::cout << "\n";
    for (int i = 0; i < 5; i++) {
        std::cout << std::left << std::setw(slotWidth)
            << (this->enemyCards[i] != nullptr ? "Defense: " + std::to_string(this->enemyCards[i]->defense) : "~");
    }
    std::cout << "\n";
    for (int i = 0; i < 5; i++) {
        std::cout << std::left << std::setw(slotWidth)
            << (this->enemyCards[i] != nullptr ? "Health: " + std::to_string(this->enemyCards[i]->currHealth) : "~");
    }
    std::cout << "\n";
    std::cout << "\n\n";

    // Player Cards
    std::cout << "Player Cards:\n";

    for (int i = 0; i < 5; i++) {
        if (this->playerCards[i] != nullptr) {
            std::cout << std::left << std::setw(slotWidth)
                << this->playerCards[i]->getType().name;
        }
        else {
            std::cout << std::left << std::setw(slotWidth) << "Empty";
        }
    }
    std::cout << "\n";
    // Print attributes
    for (int i = 0; i < 5; i++) {
        std::cout << std::left << std::setw(slotWidth)
            << (this->playerCards[i] != nullptr ? "Attack: " + std::to_string(this->playerCards[i]->attack) : "~");
    }
    std::cout << "\n";
    for (int i = 0; i < 5; i++) {
        std::cout << std::left << std::setw(slotWidth)
            << (this->playerCards[i] != nullptr ? "Defense: " + std::to_string(this->playerCards[i]->defense) : "~");
    }
    std::cout << "\n";
    for (int i = 0; i < 5; i++) {
        std::cout << std::left << std::setw(slotWidth)
            << (this->playerCards[i] != nullptr ? "Health: " + std::to_string(this->playerCards[i]->currHealth) : "~");
    }
    std::cout << "\n\n";
    std::cout << "Player Hand:\n";
    for (Card* card : this->playerHand) {
        std::cout << card->getType().name << "\n";
    }
    std::cout << "\n";
    std::cout << "Player Deck: " << this->playerDeck.size() << " Cards\n";
    std::cout << "Player Health: " << this->playerHealth << "\n";
}

void Board::drawCard(bool isPlayer) {
    std::vector<Card*>& deck = isPlayer ? this->playerDeck : this->enemyDeck;
    std::vector<Card*>& hand = isPlayer ? this->playerHand : this->enemyHand;
    if (deck.empty()) {
        std::cout << "Deck is empty\n";
        return;
    }
    Card* card = deck[0];
    deck.erase(deck.begin());
    hand.push_back(card);
}

bool Board::playCard(bool isPlayer, int cardIndex, int pos) {
    std::vector<Card*>& targetCards = isPlayer ? this->playerCards : this->enemyCards;
    std::vector<Card*>& handCards = isPlayer ? this->playerHand : this->enemyHand;

    if (pos < 0 || pos >= 5) {
        std::cout << "Invalid position\n";
        return false;
    }
    if (targetCards[pos] != nullptr) {
        std::cout << "Position already occupied\n";
        return false;
    }
    if (cardIndex < 0 || cardIndex >= handCards.size()) {
        std::cout << "Invalid card index\n";
        return false;
    }

    // TODO: Detect for "surprise" cards on opposite side

    targetCards[pos] = handCards[cardIndex];
    handCards.erase(handCards.begin() + cardIndex);
    return true;
}

Director::Director() : board() {
    this->startGame();
}


void Director::initializeDecks() {

    auto playerAdd = [this](CardID id) {
        this->board.playerDeck.push_back(new Card(Board::cardRegistry[id]));
        };
    auto enemyAdd = [this](CardID id) {
        this->board.enemyDeck.push_back(new Card(Board::cardRegistry[id]));
        };

    this->board.playerDeck.clear();
    this->board.enemyDeck.clear();

    playerAdd(ARAGORN);
    playerAdd(ELVEN_SON);
    for (int i = 0; i < 10; i++) {
        playerAdd(RECRUIT);
    }

    enemyAdd(BATTERING_RAM);
    for (int i = 0; i < 10; i++) {
        enemyAdd(URUK);
        enemyAdd(ORC);
    }
}

void Director::startGame() {
    this->board.resetAssault();
    this->initializeDecks();
    this->shuffleDeck(true);
    this->shuffleDeck(false);

    // Draw Cards
    this->drawCards(true, 5);
    this->drawCards(false, 5);
}
void Director::endGame() {
    std::cout << "Game ended\n";
}

void Director::shuffleDeck(bool isPlayer) {
    std::vector<Card*>* deck = isPlayer ? &this->board.playerDeck : &this->board.enemyDeck;
    std::random_shuffle(deck->begin(), deck->end());
}

bool Director::playCard(bool isPlayer, int cardIndex, int pos) {

    // Detect if the card is placed on defense or not. On defense, cards can only be placed to block other cards
    if ((isPlayer && !first) || (!isPlayer && first)) {
        Card* oppositeCard = isPlayer ? board.enemyCards[pos] : board.playerCards[pos];
        if (oppositeCard == nullptr) { // No attacking card
            std::cout << "No attacking card\n";
            return false;
        }
    }

    if (this->board.playCard(isPlayer, cardIndex, pos)) {
        return true;
    }
    else {
        std::cout << "Failed to play card\n";
        return false;
    }
}

void Director::drawCards(bool isPlayer, int targetCards) {
    std::vector<Card*>& deck = isPlayer ? this->board.playerDeck : this->board.enemyDeck;
    std::vector<Card*>& hand = isPlayer ? this->board.playerHand : this->board.enemyHand;
    while (hand.size() < targetCards && !deck.empty()) {
        drawCard(isPlayer);
    }
}

bool Director::turnAttack() {
    for (int i = 0; i < 5; i++) {
        Card* enemyCard = this->board.enemyCards[i];
        Card* playerCard = this->board.playerCards[i];

        if (enemyCard == nullptr && playerCard == nullptr) {
            continue; // Skip if both cards are null
        }

        if (playerCard == nullptr) {
            board.playerHealth -= enemyCard->attack;
            continue;
        }
        else if (enemyCard == nullptr) {
            board.enemyHealth -= playerCard->attack;
            continue;
        }

        applySpecialAbilities(playerCard, enemyCard); // Apply special abilities if any
        playerCard->currHealth -= std::max(0, enemyCard->attack - playerCard->defense);
        enemyCard->currHealth -= std::max(0, playerCard->attack - enemyCard->defense);
        if (playerCard->currHealth <= 0) {
            delete playerCard;
            this->board.playerCards[i] = nullptr;
        }
        if (enemyCard->currHealth <= 0) {
            delete enemyCard;
            this->board.enemyCards[i] = nullptr;
        }

        // Reset the attack and defense values for the next turn, in case they've changed
        enemyCard->attack = enemyCard->getType().attack;
        playerCard->attack = playerCard->getType().attack;
        enemyCard->defense = enemyCard->getType().defense;
        playerCard->defense = playerCard->getType().defense;
    }

    if (board.playerHealth > 0 && board.enemyHealth > 0) {
        return true;
    }
    else {
        return false;
    }
}