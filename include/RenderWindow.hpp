#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>

#include "Math.hpp"
#include "Entity.hpp"

extern const int SCREEN_X, SCREEN_Y;

class RenderWindow
{
private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
public:
    RenderWindow(const char* p_title, int p_width, int p_height);
    SDL_Texture* loadTexture(const char* p_filePath);
    void clear();
    void color(ImVec4 color);
    void render(Entity& p_entity);
    void renderText(float p_x, float p_y, const char* p_text, TTF_Font* p_font, SDL_Color p_textColor, bool closeFont = false);
    void renderTextCenter(float p_x, float p_y, const char* p_text, TTF_Font* p_font, SDL_Color p_textColor, bool closeFont = false);
    void display();
    void cleanUp();
};

RenderWindow::RenderWindow(const char* p_title, int p_width, int p_height)
{
    window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, p_width, p_height, SDL_WINDOW_SHOWN);
    if (window == nullptr)
        std::cout << "SDL_CreateWindow ERROR: " << SDL_GetError() << std::endl;
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
        std::cout << "SDL_CreateRenderer ERROR: " << SDL_GetError() << std::endl;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.Fonts->AddFontFromFileTTF("resources/HANBatangB.ttf", 20.0f, NULL, io.Fonts->GetGlyphRangesKorean());
    io.FontDefault = io.Fonts->Fonts[0];

    ImGui::StyleColorsDark();

    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer_Init(renderer);
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath)
{
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(renderer, p_filePath);

    if (texture == NULL)
        std::cout << "IMG_LoadTexture ERROR: " << SDL_GetError() << std::endl;

    return texture;
}

void RenderWindow::clear()
{
    SDL_RenderClear(renderer);
}

void RenderWindow::color(ImVec4 color)
{
    SDL_SetRenderDrawColor(renderer, color.x*255, color.y*255, color.z*255, color.w*255);
}

void RenderWindow::render(Entity& p_entity)
{
    SDL_Rect src = p_entity.getFrame();

    Vector2f position = p_entity.getPosition();
    Vector2f scale = p_entity.getScale();
    SDL_Rect dst;
    dst.x = position.x;
	dst.y = position.y;
	dst.w = src.w * scale.x;
	dst.h = src.h * scale.y;

    SDL_RenderCopyEx(renderer, p_entity.getTexture(), &src, &dst, p_entity.getRotate(), NULL, p_entity.getFlip());
}

void RenderWindow::renderText(float p_x, float p_y, const char* p_text, TTF_Font* p_font, SDL_Color p_textColor, bool closeFont)
{
	SDL_Surface* renderText = TTF_RenderUTF8_Blended(p_font, p_text, p_textColor);
	SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, renderText);

	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	src.w = renderText->w;
	src.h = renderText->h; 

	SDL_Rect dst;
	dst.x = p_x - renderText->w/2;
	dst.y = p_y - renderText->h/2;
	dst.w = src.w;
	dst.h = src.h;

    SDL_SetTextureAlphaMod(message, p_textColor.a);
	SDL_RenderCopy(renderer, message, &src, &dst);
	SDL_FreeSurface(renderText);
	SDL_DestroyTexture(message);
    if (closeFont)
    {
        TTF_CloseFont(p_font);
    }
}

void RenderWindow::renderTextCenter(float p_x, float p_y, const char* p_text, TTF_Font* p_font, SDL_Color p_textColor, bool closeFont)
{
	SDL_Surface* renderText = TTF_RenderUTF8_Blended(p_font, p_text, p_textColor);

	int x = SCREEN_X/2 + p_x;
	int y = SCREEN_Y/2 + p_y;

    SDL_FreeSurface(renderText);
    RenderWindow::renderText(x, y, p_text, p_font, p_textColor, closeFont);
}

void RenderWindow::display()
{
    SDL_RenderPresent(renderer);
}

void RenderWindow::cleanUp()
{
    SDL_DestroyWindow(window);
}