/*
Just a list of colors that can be used in the game
*/

#pragma once

typedef struct Color {
    Uint8 r, g, b, a = 255;

    bool operator==(const Color& other) const {
        return (r == other.r && g == other.g && b == other.b && a == other.a);
    }

    inline Color alpha(Uint8 alpha) const {
        return Color(r, g, b, alpha);
    }
} Color;


const Color CLEAR = { 0, 0, 0, 0 };
const Color BLACK = { 0, 0, 0, 255 };
const Color WHITE = { 255, 255, 255, 255 };
const Color OFFWHITE = { 240, 240, 240, 255 };
const Color GRAY = { 180, 180, 180, 255 };
const Color GHOST = { 150, 150, 150, 125 };
const Color RED = { 255, 0, 0, 255 };
const Color MEDIUM_RED = { 181, 0, 0, 255 };
const Color GREEN = { 0, 225, 0, 255 };
const Color MEDIUM_GREEN = { 32, 156, 0, 255 };
const Color DARK_GREEN = { 0, 100, 0, 255 };
const Color BLUE = { 0, 0, 255, 255 };
const Color MEDIUM_BLUE = { 0, 44, 163, 255 };
const Color YELLOW = { 255, 255, 0, 255 };
const Color CYAN = { 0, 255, 255, 255 };
const Color MAGENTA = { 255, 0, 255, 255 };
const Color ORANGE = { 255, 165, 0, 255 };
const Color PURPLE = { 128, 0, 128, 255 };
const Color PINK = { 255, 192, 203, 255 };