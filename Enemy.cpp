#include "Enemy.hpp"
#include <iostream>
#include <random>

#include "Game.hpp"


void EnemyAI::turn() {

    // Draw cards
    game->drawCards(false);

    if (!first) {
        attack();
    }
    else {
        defend();
    }
}

void EnemyAI::attack() {
    // For now, the enemy will play a random card in hand in the first empty position on the board.

    int cardIndex = rand() % hand.size();

    int pos = 0;
    while (game->playCard(false, cardIndex, pos) == false) {
        pos++;
        if (pos >= 5) {
            std::cout << "EnemyAI: No empty positions available\n";
            return;
        }
    }

    std::cout << "Enemy played " << hand[cardIndex]->getType().name << " in position " << pos + 1 << "\n";
}

void EnemyAI::defend() {
    // For now, the enemy will play a random card in hand to match any attackers

    for (int i = 0; i < 5; i++) {
        if (playerCards[i] != nullptr) { // Found attacking card -> defend against it
            std::cout << "EnemyAI: Defending against player card in position " << i + 1 << "\n";
            int cardIndex = rand() % hand.size();
            if (game->playCard(false, cardIndex, i)) {
                std::cout << "Enemy played " << cards[i]->getType().name << " in position " << i + 1 << "\n";
            }
        }
    }
}