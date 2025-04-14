/*
Just a list of colors that can be used in the game
*/

#pragma once

typedef struct Color {
    Uint8 r, g, b, alpha;
} Color;


const Color BLACK = { 0, 0, 0, 255 };
const Color WHITE = { 255, 255, 255, 255 };
const Color GRAY = { 150, 150, 150, 255 };
const Color GHOST = { 150, 150, 150, 125 };
const Color RED = { 255, 0, 0, 255 };
const Color GREEN = { 0, 255, 0, 255 };
const Color BLUE = { 0, 0, 255, 255 };
const Color YELLOW = { 255, 255, 0, 255 };
const Color CYAN = { 0, 255, 255, 255 };
const Color MAGENTA = { 255, 0, 255, 255 };
const Color ORANGE = { 255, 165, 0, 255 };
const Color PURPLE = { 128, 0, 128, 255 };
const Color PINK = { 255, 192, 203, 255 };