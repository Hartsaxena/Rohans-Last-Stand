/*
Game.hpp defines the general rules and functionality of the card game by itself. It doesn't render or simulate input or AI behavior, so it theoretically could be used to make an entirely different game.
For game rules, see Rules.txt
*/


#pragma once

#include <string>
#include <vector>
#include <map>

typedef enum CardID {
    // Player
    ARAGORN,
    ELVEN_SON,
    RECRUIT,


    // Enemy
    URUK,
    ORC,
    BATTERING_RAM,
} CardID;

/**
 * @brief The CardType struct defines the attributes of a card type.
 * It contains the name, max health, attack, and defense of the card.
 * These attributes are used to create a card instance, and are not changed during the game.
 * With this being said, game logic can temporarily change these stats. For example, buffing a card may increase its attack or defense for a turn.
 */
typedef struct CardType {
    std::string name;
    int maxHealth, attack, defense;
} CardType;

class Card {
public:
    Card(CardType type);
    ~Card() = default;

    // Temporary stats. These can change depending on conditions (armor shredding, buffing, etc.)

    // Current health (int)
    int currHealth;
    // Temporary Attack stat
    int attack;
    // Temporary Defense stat
    int defense;

    const CardType& getType() const { return type; }

private:
    CardType type;
};


/**
 * @brief The Board class is responsible for managing the game state, including the player's and enemy's cards, health, and deck.
 * It is manipulated and used by the Director class. Additionally, Director is a friend class of Board.
 */
class Board {
public:
    int enemyHealth = 20;
    int playerHealth = 20;

    Board();
    ~Board();
    void resetAssault();
    void print() const;

    void drawCard(bool isPlayer);
    bool playCard(bool isPlayer, int cardIndex, int pos);

private:
    friend class Director;

    std::vector<Card*> enemyCards; // Cards in Play
    std::vector<Card*> playerCards;

    std::vector<Card*> enemyHand; // Cards inside hand
    std::vector<Card*> playerHand;

    std::vector<Card*> enemyDeck; // Cards in deck
    std::vector<Card*> playerDeck;
    static std::map<CardID, CardType> cardRegistry;
};


/**
 * @brief The Director class is responsible for managing the game flow and interactions between the player and the enemy.
 * @note This class is not responsible for rendering or keeping track of cards on the board.
 */
class Director {
public:

    /**
     * @brief first is a boolean that indicates whose turn it is first in the round.
     * This means that first only changes at the start of a round, after both players have played their cards and an assault has happened.
     *
     * true = Player's turn first, false = Enemy's turn first.
     */
    bool first = true;

    Director();

    void initializeDecks();
    void startGame();
    void endGame();

    void shuffleDeck(bool isPlayer);
    bool playCard(bool isPlayer, int cardIndex, int pos);
    void drawCard(bool isPlayer) {
        this->board.drawCard(isPlayer);
    }
    void drawCards(bool isPlayer, int targetCards);

    void applySpecialAbilities(Card* playerCard, Card* enemyCard) {}

    /**
     * @brief turnAttack simulates the Assault phase of each round (see game rules for more details).
     * @return true if the game is still running, false if the game has ended.
     */
    bool turnAttack();

    void printBoard() {
        this->board.print();
    }

    // Get/Setters

    int getEnemyHealth() const { return this->board.enemyHealth; }
    int getPlayerHealth() const { return this->board.playerHealth; }
    int getEnemyDeckSize() const { return this->board.enemyDeck.size(); }
    int getPlayerDeckSize() const { return this->board.playerDeck.size(); }
    const std::vector<Card*>& getEnemyCards() { return this->board.enemyCards; }
    const std::vector<Card*>& getPlayerCards() { return this->board.playerCards; }
    const std::vector<Card*>& getEnemyHand() { return this->board.enemyHand; }
    const std::vector<Card*>& getPlayerHand() { return this->board.playerHand; }

private:
    Board board;
};