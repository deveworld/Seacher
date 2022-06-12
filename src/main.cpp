#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>

const int SCREEN_X = 1280;
const int SCREEN_Y = 720;

#include "RenderWindow.hpp"

bool init();
void eventHandle();
TTF_Font* getFont(int size, bool bold = false);

bool running = true;
bool mouseDown = false;
int state = 0;
SDL_Event event;
bool inited = init();

RenderWindow window("Seacher", SCREEN_X, SCREEN_Y);

SDL_Texture* blackboardTexture = window.loadTexture("resources/blackboard.png");
SDL_Texture* deskTexture = window.loadTexture("resources/desk.png");
SDL_Texture* teacherDeskTexture = window.loadTexture("resources/teacherDesk.png");

Uint64 currentTick;
Uint64 lastTick = 0;
double deltaTime = 0;

int main(int argv, char** args)
{
    if (!inited)
    {
        std::cout << "Not Inited" << std::endl;
        return 0;
    }
    
    currentTick = SDL_GetPerformanceCounter();
    while (running)
    {
        eventHandle();

        window.clear();
        window.color(150, 150, 150, 255);
        window.renderTextCenter(0, 0, "Seacher", getFont(84, true), {0, 255, 255});
        window.display();
    }

    window.cleanUp();
    SDL_Quit();

    return 0;
}

bool init()
{
    if (!(IMG_Init(IMG_INIT_PNG)))
    {
		std::cout << "IMG_Init Error: " << SDL_GetError() << std::endl;
        return false;
    }
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "SDL_Init ERROR: " << SDL_GetError() << std::endl;
        return false;
    }
    if (TTF_Init() != 0)
    {
		std::cout << "TTF_init Error: " << SDL_GetError() << std::endl;
    }
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    return true;
}

TTF_Font* getFont(int size, bool bold)
{
    return TTF_OpenFont(bold ? "resources/HANBatangB.ttf" : "resources/HANBatang.ttf", size);
}

void eventHandle()
{
    lastTick = currentTick;
	currentTick = SDL_GetPerformanceCounter();
	deltaTime = (double)((currentTick - lastTick)*1000 / (double)SDL_GetPerformanceFrequency() );
    
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            running = false;
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                mouseDown = true;
            }
            break;
        
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                mouseDown = false;
            }
            break;
        
        default:
            break;
        }
    }
}