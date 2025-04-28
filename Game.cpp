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

    // {NAME, {"Name", maxHealth, attack, defense, specialAbility, playCondition}}

    {STRIDER, {STRIDER, "Strider", 7, 7, 2, INSPIRE}},
    {ELVEN_PRINCE, {ELVEN_PRINCE, "Elven Prince", 4, 9, 0, ARMOR_PIERCE}},
    {RECRUIT, {RECRUIT, "Recruit", 3, 2, 2}},
    {ELVEN_SOLDIER, {ELVEN_SOLDIER, "Elf Soldier", 4, 4, 0, ARMOR_PIERCE}},
    {LOCKBEARER, {LOCKBEARER, "Lockbearer", 10, 6, 4, INSPIRE, DEFENSE_ONLY}},
    {THE_WHITE, {THE_WHITE, "The White", 24, 8, 0, REINFORCE}},
    {KING, {KING, "The King", 9, 5, 5, INSPIRE}},

    {URUK, {URUK, "Uruk-Hai", 3, 4, 1} },
    {ORC, {ORC, "Orc", 3, 3, 0}},
    {DUNLENDING, {DUNLENDING, "Dunlending", 3, 2, 1, HATE}},
    {BERSERKER, {BERSERKER, "Berserker", 9, 5, 3, RALLY}}
    {BATTERING_RAM, {BATTERING_RAM, "Battering Ram", 2, 8, 1, SURPRISE, ATTACK_ONLY}},
    {FELGROM, {FELGROM, "Felgrom", 1, 10, 1, KAMIKAZE, ATTACK_ONLY}},
};

std::map<CardID, int> Board::playerDeckRegistry = {
    {STRIDER, 2},
    {ELVEN_PRINCE, 2},
    {RECRUIT, 10},
    {ELVEN_SOLDIER, 5},
    {LOCKBEARER, 2},
    {THE_WHITE, 1},
    {KING, 1}
};

std::map<CardID, int> Board::enemyDeckRegistry = {
    {URUK, 10},
    {ORC, 20},
    {DUNLENDING, 10},
    {BERSERKER, 8},
    {BATTERING_RAM, 2},
    {FELGROM, 2},
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

    // Check if the position is valid
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

    // All conditions met. Play card.
    targetCards[pos] = handCards[cardIndex];
    handCards.erase(handCards.begin() + cardIndex);
    return true;
}

Director::Director() : board() {
    this->startGame();
}


void Director::initializeDecks() {

    this->board.playerDeck.clear();
    this->board.enemyDeck.clear();

    for (const auto& card : Board::playerDeckRegistry) {
        for (int i = 0; i < card.second; i++) {
            this->board.playerDeck.push_back(new Card(Board::cardRegistry.at(card.first)));
        }
    }

    for (const auto& card : Board::enemyDeckRegistry) {
        for (int i = 0; i < card.second; i++) {
            this->board.enemyDeck.push_back(new Card(Board::cardRegistry.at(card.first)));
        }
    }
}

void Director::startGame() {
    this->board.resetAssault();
    this->initializeDecks();
    this->shuffleDeck(true);
    this->shuffleDeck(false);

    // Draw Cards
    this->drawCards(true, maxCards);
    this->drawCards(false, maxCards);
}
void Director::endGame() {
    std::cout << "Game ended\n";
}

void Director::shuffleDeck(bool isPlayer) {
    std::vector<Card*>* deck = isPlayer ? &this->board.playerDeck : &this->board.enemyDeck;
    std::mt19937 rng(std::random_device{}());
    std::shuffle(deck->begin(), deck->end(), rng);
}

bool Director::playCard(bool isPlayer, int cardIndex, int pos) {
    Card* card = isPlayer ? this->board.playerHand[cardIndex] : this->board.enemyHand[cardIndex];

    // Detect if the card is placed on defense or not. On defense, cards can only be placed to block other cards
    if ((isPlayer && !first) || (!isPlayer && first)) {
        Card* oppositeCard = isPlayer ? board.enemyCards[pos] : board.playerCards[pos];

        // Check if card has ATTACK_ONLY condition
        if (card->getType().condition == ATTACK_ONLY) {
            std::cout << "Cannot play card with ATTACK_ONLY condition on defense\n";
            return false;
        }

        // No attacking card. On defense, cards can only be placed to block other cards
        if (oppositeCard == nullptr) {
            std::cout << "No attacking card\n";
            return false;
        }

        // Check if attacking card has the SURPRISE special ability
        else if (oppositeCard->getType().special == SURPRISE) {
            std::cout << "Cannot play card on defense against attacking card with SURPRISE special ability\n";
            return false;
        }
    }
    else { // On attack, cards can be placed anywhere
        // Check if card has DEFENSE_ONLY condition
        if (card->getType().condition == DEFENSE_ONLY) {
            std::cout << "Cannot play card with DEFENSE_ONLY condition on attack\n";
            return false;
        }
    }


    if (this->board.playCard(isPlayer, cardIndex, pos)) {

        // Card played successfully. Apply special abilities if any
        switch (card->getType().special) {
        case INSPIRE: {
            std::cout << "Inspiring card played\n";
            // Apply the INSPIRE special ability
            for (int i = 0; i < 5; i++) {
                if (this->board.playerCards[i] != nullptr) {
                    this->board.playerCards[i]->attack += 1;
                }
            }
            break;
        }
        }

        return true;
    }
    else {
        std::cout << "Failed to play card\n";
        return false;
    }
}

void Director::discardCard(bool isPlayer, int cardIndex) {
    std::vector<Card*>& hand = isPlayer ? this->board.playerHand : this->board.enemyHand;
    if (cardIndex < 0 || cardIndex >= hand.size()) {
        std::cout << "Invalid card index\n";
        return;
    }

    // Remove from the board
    std::vector<Card*>& targetCards = isPlayer ? this->board.playerCards : this->board.enemyCards;
    targetCards[cardIndex] = nullptr;

    // Add back to bottom of the deck
    std::vector<Card*>& deck = isPlayer ? this->board.playerDeck : this->board.enemyDeck;
    deck.push_back(hand[cardIndex]);

    // Remove from hand
    hand.erase(hand.begin() + cardIndex);
}

void Director::drawCards(bool isPlayer, int targetCards) {
    std::vector<Card*>& deck = isPlayer ? this->board.playerDeck : this->board.enemyDeck;
    std::vector<Card*>& hand = isPlayer ? this->board.playerHand : this->board.enemyHand;
    while (hand.size() < targetCards && !deck.empty()) {
        drawCard(isPlayer);
    }
}

void Director::applyAssaultAbilities(Card* attacker, Card* defender) {
    switch (attacker->getType().special) {
    case ARMOR_PIERCE: {
        std::cout << "Armor Pierce card played\n";
        // Apply the ARMOR_PIERCE special ability
        defender->defense = 0; // Set enemy card's defense to 0
        break;
    };
    case KAMIKAZE: {
        std::cout << "Kamikaze card played\n";
        // Apply the KAMIKAZE special ability
        attacker->currHealth = 0; // Destroy the card
        break;
    };
    default: break;
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

        applyAssaultAbilities(playerCard, enemyCard); // Apply assault special abilities if any
        applyAssaultAbilities(enemyCard, playerCard);
        playerCard->currHealth -= std::max(0, enemyCard->attack - playerCard->defense);
        enemyCard->currHealth -= std::max(0, playerCard->attack - enemyCard->defense);
        if (playerCard->currHealth <= 0) {
            discardCard(true, i);
        }
        if (enemyCard->currHealth <= 0) {
            discardCard(false, i);
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
        board.playerHealth = std::max(0, board.playerHealth);
        board.enemyHealth = std::max(0, board.enemyHealth);
        return false;
    }

    shuffleDeck(true);
    shuffleDeck(false);
}