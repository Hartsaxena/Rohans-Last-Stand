// Enemy.cpp
#include "Enemy.hpp"
#include <iostream>

#include "Game.hpp"

void EnemyAI::turn() {
    // Draw up to hand limit
    game->drawCards(false);

    if (!first) {
        attack();
    }
    else {
        defend();
    }
}

void EnemyAI::attack() {
    // Fill every empty attack slot
    for (int pos = 0; pos < cards.size(); ++pos) {

        if (cards[pos] != nullptr) {
            // This slot is already filled
            continue;
        }

        // Only attack 3/4 of the time
        if (rand() % 5 == 0) {
            std::cout << "Enemy skips attacking slot " << pos + 1 << "\n";
            continue;
        }

        // Keep trying to play into this pos until either it's full or no playable cards remain
        while (cards[pos] == nullptr) {

            bool played = false;
            for (int i = 0; i < hand.size(); ++i) {
                Card* c = hand[i];
                // Skip cards that can't attack
                if (c->getType().condition == DEFENSE_ONLY)
                    continue;
                if (game->playCard(false, i, pos)) {
                    std::cout << "Enemy plays "
                        << c->getType().name
                        << " at slot " << pos + 1 << "\n";
                    played = true;
                    break;
                }
            }
            if (!played) break;  // no more valid plays for this slot
        }
    }
}

void EnemyAI::defend() {
    // Block most incoming attackers
    for (int pos = 0; pos < playerCards.size(); pos++) {
        if (playerCards[pos] == nullptr && cards[pos] != nullptr) {
            // No need to defend if there's no attacker
            continue;
        }

        // Only defend 4/5 of the time
        if (rand() % 5 == 0) {
            std::cout << "Enemy skips defending slot " << pos + 1 << "\n";
            continue;
        }
        for (int i = 0; i < hand.size(); ++i) {
            if (game->playCard(false, i, pos)) {
                std::cout << "Enemy defends slot " << pos + 1
                    << " with " << hand[i]->getType().name << "\n";
                break;
            }
        }
    }
}
