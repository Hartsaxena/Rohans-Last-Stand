#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "Colors.hpp"

typedef struct Rectangle {
    /* Rectangle structure */
    SDL_Rect rect;
    Color color;
    int& x = rect.x;
    int& y = rect.y;
    int& w = rect.w;
    int& h = rect.h;

    bool snap(int minX, int minY, int maxX, int maxY);
    bool snap(int maxX, int maxY);
} Rectangle;

typedef struct Position {
    int x;
    int y;
} Position;


/**
 * @brief A Surface object is responsible for managing rendering over a small, movable area.
 * It should be used to draw multiple objects onto a group of pixels so that all of the objects can be moved together on the screen.
 */
class Surface;


/**
 * @brief A Font object is responsible for managing a loaded font.
 * It should be reuse whenever the font is needed to render text.
 */
class Font {
public:
    int size;

    Font(const std::string& fontPath, int size);
    ~Font() { TTF_CloseFont(font); };

	TTF_Font* getFont() const { return font; }

private:
	TTF_Font* font;
};

/**
 * @brief A Canvas object is responsible for managing the rendering of the game.
 * It should be used as an accessor to a SDL_Renderer object.
 */
class Canvas {

public:
    SDL_Renderer* renderer;
    int xDimension, yDimension;

    /**
     * @brief Creates a new Canvas object.
     * @param renderer Pointer to a SDL_Renderer object, which is used beforehand.
     */
    explicit Canvas(SDL_Renderer* renderer);
    ~Canvas() {}

    /********* DRAWING BASIC SHAPES **********/

    /**
     * @brief Fills the screen entirely with a color.
     * @param color Color object, representing the color to be drawn on the screen.
     */
    void fillScreenColor(Color color) const;

    /**
     * @brief Fills the screen with black (0, 0, 0, 255).
     */
    void blankScreen() const;
    /**
     * @brief Draws a rectangle on to the screen.
     * @param rect Pointer to the Rectangle object to be drawn.
     */
    void drawRect(Rectangle* rect) const;
    /**
     * @brief Draws an empty rectangle
     * @param emptyrect Pointer to a Rectangle object representing the empty rectangle to be drawn.
     */
    void drawEmptyRect(Rectangle* emptyRect) const;
    /**
     * @brief Draws a straight line on the screen.
     * @param start Position object representing one of the line's endpoints.
     * @param end Position object representing the other endpoint.
     * @param color Color of the line to be drawn.
     */
    void drawLine(Position start, Position end, Color color) const;
    /**
     * @brief Identical to drawLine(Position, Position, Color), but with integer arguments.
     * Draws a straight line on the screen.
     * @param x1 X-coordinate of the first endpoint.
     * @param y1 Y-coordinate of the first endpoint.
     * @param x2 X-coordinate of the second endpoint.
     * @param y2 Y-coordinate of the second endpoints
     * @param color Color of the line to be drawn
     */
    void drawLine(int x1, int y1, int x2, int y2, Color color) const {
        drawLine({ x1, y1 }, { x2, y2 }, color);
    }


    /********* RENDERING TEXT **********/

    /**
     * @brief Presents text to the screen.
     * @param text Text to be rendered.
     * @param font Font object to use for rendering the text.
	 * @param x X-coordinate of the text's top-left corner.
     * @param y Y-coordinate of the text's top-left corner.
	 * @param color Color object representing the color of the text.
     */
    void renderText(const std::string text, Font* font, int x, int y, Color color) const;


    /********* RENDERING IMAGES AND SURFACES **********/

    /**
     * @brief
     * @param surface Pointer a Surface object, to be blitted onto the screen.
     * @param position Position object, representing the coordinates of the surface's top-left corner on the screen.
     */
    void blitSurface(Surface* surface, Position position) {};

private:

    // Private helper functions
    void setColor(Color color) const { SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.alpha); }
};