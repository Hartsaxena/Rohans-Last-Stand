/*
This file defines the Enemy AI for the game. The AI is responsible for making decisions on which cards to play and when to attack.
*/


#include "Game.hpp"

class EnemyAI {
public:
    EnemyAI(Director* game) : game(game),
        cards(game->getEnemyCards()),
        playerCards(game->getPlayerCards()),
        hand(game->getEnemyHand()),
        first(game->first)  // Reference to the first turn
    {
    }

    void turn();
    void attack();
    void defend();

private:
    Director* game;
    const std::vector<Card*>& cards;
    const std::vector<Card*>& playerCards; // Player's assault cards
    const std::vector<Card*>& hand;
    const bool& first;
};