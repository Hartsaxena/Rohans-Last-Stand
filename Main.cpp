/*
Driver file.
*/

#include <iostream>
#include <cstdlib>

#include "SDLConnector.hpp"
#include "Game.hpp"
#include "Enemy.hpp"

//int main(int argc, char* argv[]) {
//    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation
//
//    SDLConnector connector(1920, 1080, 60, "Rohan's Last Stand");
//
//    bool isRunning = true;
//    while (isRunning) {
//        isRunning = connector.tick();
//    }
//
//    return 0;
//}

int main(int argc, char* argv[]) { // No graphics
    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation

    Director game;
    EnemyAI enemy(&game);

    bool isRunning = true;
    while (isRunning) {
        if (!game.first) { // Enemy's turn first
            enemy.turn();
        }

        // Player's turn

        game.drawCards(true, 5); // Draw until 5 cards are in hand

        int action = 0;
        while (action != 2) {
            system("cls");
            game.printBoard();
            std::cout << "Your turn! Choose an action: (1: Play Card, 2: End Turn): ";
            std::cin >> action;

            switch (action) {
            case 1: {
                int cardIndex, pos;
                std::cout << "Enter card index to play (1-5): ";
                std::cin >> cardIndex;
                std::cout << "Enter position to play the card (1-5): ";
                std::cin >> pos;
                // Note: Values are 1-indexed in the prompt, but we need to convert them to 0-indexed for our internal representation
                if (game.playCard(true, cardIndex - 1, pos - 1)) {
                    std::cout << "Card played successfully!\n";
                }
                else {
                    std::cout << "Failed to play card. Try again.\n";
                }
                break;
            }
            case 2: {
                continue;
            }
            default:
                std::cout << "Invalid action. Try again.\n";
                continue;
            }
        }

        if (game.first) { // Player's turn first
            enemy.turn();
        }
        game.printBoard();
        std::cin.get();

        // Both players have played their turns now. Start the assault sequence
        std::cout << "\nAssault phase!\n";
        std::cout << "Press Enter to continue...\n";
        std::cin.get();
        system("cls");
        isRunning = game.turnAttack();
        game.printBoard();

        std::cout << "End of Round. Press Enter to continue...\n";
        game.first = !game.first; // Switch turns
        std::cin.get();
    }

    // Game has ended
    if (game.getPlayerHealth() <= 0) {
        std::cout << "You lose!\n";
    }
    else if (game.getEnemyHealth() <= 0) {
        std::cout << "You win!\n";
    }
    else {
        std::cout << "Draw!\n"; // This case should not happen
    }

    return 0;

}