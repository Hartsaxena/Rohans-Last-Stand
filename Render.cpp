/*
This file contains methods that render all objects in the game. Calculations for updating positions of objects, and the like, are done in the update.c file. Game updates and rendering should be done separately.
*/
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "Front.hpp"
#include "Render.hpp"
#include "Colors.hpp"


bool Rectangle::snap(int minX, int minY, int maxX, int maxY) {
    bool snapped = false;
    if (this->x < minX) { this->x = minX; snapped = true; }
    if (this->y < minY) { this->y = minY; snapped = true; }
    if (this->x + this->w > maxX) { this->x = maxX - this->w; snapped = true; }
    if (this->y + this->h > maxY) { this->y = maxY - this->h; snapped = true; }
    return snapped;
}
bool Rectangle::snap(int maxX, int maxY) {
    return this->snap(0, 0, maxX, maxY);
}

Font::Font(const std::string& fontPath, int size) {
    this->font = TTF_OpenFont(fontPath.c_str(), size);
    this->size = size;
    if (this->font == NULL) {
        std::cout << "Failed to load font: " << fontPath << std::endl;
        std::cout << TTF_GetError() << std::endl;
        exit(1);
    }
}

Canvas::Canvas(SDL_Renderer* renderer)
{
    // Constructor
    this->renderer = renderer;
    SDL_GetRendererOutputSize(renderer, &xDimension, &yDimension);
}


void Canvas::fillScreenColor(Color color) const
{
    // Fills the screen with a color.
    setColor(color);
    SDL_RenderClear(renderer);
}

void Canvas::blankScreen() const
{
    // Fills the screen with black
    this->fillScreenColor(BLACK);
}

void Canvas::drawRect(const Rectangle* rect) const
{
    this->drawEmptyRect(rect);
    SDL_RenderFillRect(renderer, &(rect->rect));
}

void Canvas::drawEmptyRect(const Rectangle* emptyRect) const
{
    Color color = emptyRect->color;
    setColor(color);
    SDL_RenderDrawRect(renderer, &(emptyRect->rect));
}

void Canvas::drawLine(Position start, Position end, Color color) const {
    setColor(color);
    SDL_RenderDrawLine(renderer, start.x, start.y, end.x, end.y);
}

void Canvas::drawTextBox(TextBox* textBox) const {
    int x = textBox->x;
    int y = textBox->y;
    int w = textBox->w;
    int h = textBox->h;
    int bsize = textBox->borderSize;
    drawRect(x - bsize, y - bsize, w + (2 * bsize), h + (2 * bsize), textBox->borderColor);
    drawRect(textBox);
    renderTextCenter(
        textBox->text, textBox->font, x + w / 2, y + h / 2, textBox->textColor
    );
}

void Canvas::drawTextCounter(TextCounter* counter) const {
    counter->setText(std::to_string(*counter->count));
    this->drawTextBox(counter);
}

void Canvas::renderText(const std::string text, Font* font, int x, int y, Color color) const {
    // Create texture for text
    SDL_Color colorObj = { color.r, color.g, color.b, color.a };
    SDL_Surface* surface = TTF_RenderText_Solid(font->getFont(), text.c_str(), colorObj);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);

    // Set position and rectangle of text
    int textW = 0;
    int textH = 0;
    SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
    SDL_Rect dstrect = { x, y, textW, textH };

    // Render text to canvas
    SDL_RenderCopy(renderer, texture, NULL, &dstrect);

    // Destroy temporary objects
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void Canvas::renderTextCenter(const std::string text, Font* font, int x, int y, Color color) const {
    SDL_Surface* surface = TTF_RenderText_Solid(font->getFont(), text.c_str(), { color.r, color.g, color.b });
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    int textWidth, textHeight;
    TTF_SizeText(font->getFont(), text.c_str(), &textWidth, &textHeight);
    SDL_Rect destRect = { x - textWidth / 2, y - textHeight / 2, textWidth, textHeight };
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    SDL_DestroyTexture(texture);
}