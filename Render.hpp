#pragma once

#include <string>
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

    Rectangle(SDL_Rect rect, Color color) : rect(rect), color(color) {}
    Rectangle(int x, int y, int w, int h, Color color) : Rectangle({ x, y, w, h }, color) {}

    bool snap(int minX, int minY, int maxX, int maxY);
    bool snap(int maxX, int maxY);

    // **this** is the crucial part**:**
    // re-bind all references to _this_ object’s rect.*
    Rectangle(const Rectangle& o)
        : rect(o.rect)
        , color(o.color)
        , x(rect.x)
        , y(rect.y)
        , w(rect.w)
        , h(rect.h)
    {
    }

    Rectangle(Rectangle&& o) noexcept
        : rect(o.rect)
        , color(o.color)
        , x(rect.x)
        , y(rect.y)
        , w(rect.w)
        , h(rect.h)
    {
    }

    Rectangle& operator=(const Rectangle& o) {
        if (this != &o) {
            rect = o.rect;
            color = o.color;
            // x,y,w,h are already bound to this->rect.*
        }
        return *this;
    }
} Rectangle;

/**
 * @brief A Font object is responsible for managing a loaded font.
 * It should be reuse whenever the font is needed to render text.
 */
class Font {
public:
    int size;

    Font(const std::string& fontPath, int size);
    ~Font() {
        if (font && TTF_WasInit()) {
            TTF_CloseFont(font);
        }
    }


    TTF_Font* getFont() const { return font; }

private:
    TTF_Font* font;
};


class Canvas;
/**
 * @brief TextBox is a rectangle that can display text.
 */
class TextBox : public Rectangle {
public:
    std::string text;
    Color textColor;
    int borderSize = 2;
    Color borderColor;

    TextBox(int x, int y, int w, int h, std::string text, Font* font, Color rectColor = CLEAR, Color borderColor = BLACK, Color textColor = OFFWHITE)
        : Rectangle(x, y, w, h, rectColor),
        text(text),
        font(font),
        borderColor(borderColor),
        textColor(textColor)
    {
    }

    void setText(const std::string& text) {
        this->text = text;
    }
    void setFont(Font* font) {
        this->font = font;
    }

private:
    Font* font;

    friend class Canvas;
};


/**
 * @brief TextCounter is a special type of TextBox that displays a counter for an integer variable.
 * @note WIP
 */
class TextCounter : public TextBox {
public:

    TextCounter(int x, int y, int w, int h, int* count, Font* font, Color rectColor = CLEAR, Color textColor = OFFWHITE)
        : TextBox(x, y, w, h, std::to_string(*count), font, rectColor, textColor),
        count(count)
    {
    }

    void setCounter(int* count) {
        this->count = count;
    }

private:
    int* count;

    friend class Canvas;
};

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
    void drawRect(const Rectangle* rect) const;
    void drawRect(int x, int y, int w, int h, Color color) const {
        Rectangle rect = { { x, y, w, h }, color };
        drawRect(&rect);
    }
    /**
     * @brief Draws an empty rectangle
     * @param emptyrect Pointer to a Rectangle object representing the empty rectangle to be drawn.
     */
    void drawEmptyRect(const Rectangle* emptyRect) const;
    void drawEmptyRect(int x, int y, int w, int h, Color color) const {
        Rectangle rect = { { x, y, w, h }, color };
        drawEmptyRect(&rect);
    }
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

    /**
     * @brief Draws a TextBox object on the screen.
     * @param textBox
     */
    void drawTextBox(TextBox* textBox) const;
    /**
     * @brief Draws a TextBox object on the screen.
     * @param x X-coordinate of the TextBox's top-left corner.
     * @param y Y-coordinate of the TextBox's top-left corner.
     * @param w Width of the TextBox.
     * @param h Height of the TextBox.
     * @param text Text to be displayed inside the TextBox.
     * @param font Font object to use for rendering the text.
     * @param rectColor Color of the TextBox's rectangle.
     * @param textColor Color of the text inside the TextBox.
     */
    void drawTextBox(int x, int y, int w, int h, std::string text, Font* font,
        Color rectColor = CLEAR, Color textColor = OFFWHITE) const {
        TextBox textBox = { x, y, w, h, text, font, rectColor, textColor };
        drawTextBox(&textBox);
    }

    /**
     * @brief Draws a TextCounter object on the screen.
     * @param counter Pointer to a TextCounter object to be drawn.
     */
    void drawTextCounter(TextCounter* counter) const;
    /**
     * @brief Draws a TextCounter object on the screen.
     * @param x X-coordinate of the TextCounter's top-left corner.
     * @param y Y-coordinate of the TextCounter's top-left corner.
     * @param w Width of the TextCounter.
     * @param h Height of the TextCounter.
     * @param count Pointer to an integer variable to be displayed inside the TextCounter.
     * @param font Font object to use for rendering the text.
     * @param rectColor Color of the TextCounter's rectangle.
     * @param textColor Color of the text inside the TextCounter.
     */
    void drawTextCounter(int x, int y, int w, int h, int* count, Font* font,
        Color rectColor = CLEAR, Color textColor = OFFWHITE) const {
        TextCounter counter = { x, y, w, h, count, font, rectColor, textColor };
        drawTextCounter(&counter);
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
    /**
     * @brief Presents text to the screen, but centered on x and y coordinates.
     * @param text Text to be rendered
     * @param font Font object to use for rendering the text.
     * @param x X-coordinate of the text's center.
     * @param y Y-coordinate of the text's center.
     * @param color Color object representing the color of the text.
     */
    void renderTextCenter(const std::string text, Font* font, int x, int y, Color color) const;


    /********* RENDERING IMAGES AND SURFACES **********/

    /**
     * @brief
     * @param surface Pointer a Surface object, to be blitted onto the screen.
     * @param position Position object, representing the coordinates of the surface's top-left corner on the screen.
     */
    void blitSurface(Surface* surface, Position position) {};

private:

    // Private helper functions
    void setColor(Color color) const { SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a); }
};


// Game specific classes - messy but I don't care anymore
class Button : public Rectangle {
public:
    /**
     * @brief Buttons (in this game) are rectangles that can be clicked.
     * @param x X position of the button.
     * @param y Y position of the button.
     * @param w Width of the button.
     * @param h Height of the button.
     * @param color Color of the button.
     */
    Button(int x, int y, int w, int h, Color color = CLEAR)
        : Rectangle(x, y, w, h, color) {
    }

    /**
     * @brief Checks if the button is clicked.
     * @param mx X position of the mouse.
     * @param my Y position of the mouse.
     * @return True if the mouse is inside the button, false otherwise.
     */
    bool collision(int mx, int my) const {
        return (mx >= x && mx <= x + w &&
            my >= y && my <= y + h);
    }

    void setColor(Color color) {
        this->color = color;
    }

    virtual void render(Canvas* canvas) {
        canvas->drawRect(this);
    }
};

class RenderableButton : public Button {
public:
    std::string text;
    Color defaultColor;
    Color hoveredColor;
    Color borderColor = BLACK;
    Color textColor = OFFWHITE;
    Color textHoverColor = OFFWHITE; // Color of the text when hovered
    bool hovered = false;

    Font* font = nullptr; // Font for rendering text

    explicit RenderableButton(int x, int y, int w, int h, Color defaultColor = MEDIUM_BLUE, Color hoveredColor = MEDIUM_GREEN)
        : Button(x, y, w, h, defaultColor),
        defaultColor(defaultColor),
        hoveredColor(hoveredColor)
    {
    }

    void addText(const std::string& text, Font* font) {
        this->text = text;
        this->font = font;
    }

    void changeText(const std::string& text) {
        this->text = text;
    }
    void changeFont(Font* font) {
        this->font = font;
    }
    void changeTextColor(Color color) {
        this->textColor = color;
    }
    void changeTextHoverColor(Color color) {
        this->textHoverColor = color;
    }

    void render(Canvas* canvas) override;
};