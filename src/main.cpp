#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
// #include <chrono>
// #include <cstdint>

// uint64_t getTime() {
//   using namespace std::chrono;
//   return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
// }

const int SCREEN_X = 1200;
const int SCREEN_Y = 900;

const int STATE_START          = 0001;
const int STATE_WAIT_KEY_START = 0002;
const int STATE_PREAPARE       = 0003;

#include "RenderWindow.hpp"
#include "Desk.hpp"

bool init();
void eventHandle();
TTF_Font* getFont(int size, bool bold = false);

bool running = true;
int state = STATE_START;
clock_t timer = clock();

bool leftMouseDown = false;
bool rightMouseDown = false;

const Uint8 *key = SDL_GetKeyboardState(NULL);

SDL_Event event;
bool inited = init();

RenderWindow window("Seacher", SCREEN_X, SCREEN_Y);

SDL_Texture* blackboardTexture      = window.loadTexture("resources/blackboard.png");
SDL_Texture* deskTexture            = window.loadTexture("resources/desk.png");
SDL_Texture* teacherDeskTexture     = window.loadTexture("resources/teacherDesk.png");

std::vector<Entity> allEntity;
std::vector<Desk> desks;

TTF_Font* font28        = getFont(28);
TTF_Font* fontB28       = getFont(28, true);

Uint64 currentTick;
Uint64 lastTick         = 0;
double deltaTime        = 0;

int main(int argv, char** args)
{
    if (!inited)
    {
        std::cout << "Not Inited" << std::endl;
        return 0;
    }

    Entity blackboard     = Entity(Vector2f(0, 0), blackboardTexture, Vector2f(0.45, 0.2), 180.0);
    Entity desk           = Entity(Vector2f(0, 0), deskTexture, Vector2f(0.13, 0.13));
    Entity teacherDesk    = Entity(Vector2f(0, 0), teacherDeskTexture, Vector2f(0.13, 0.13));

    blackboard.center();
    blackboard.move(0, 438);

    teacherDesk.center();
    teacherDesk.move(0, 350);

    for (size_t i = 1; i <= 6; i++)
    {
        for (size_t j = 1; j <= 6; j++)
        {
            Desk newDesk = Desk(desk, Vector2f(i, j));
            desks.push_back(newDesk);
        }
    }
    Vector2f deskSize = desks[0].getEntity()->getSize();

    allEntity.push_back(teacherDesk);
    allEntity.push_back(blackboard);
    
    currentTick = SDL_GetPerformanceCounter();
    while (running)
    {
        eventHandle();

        // if (key[SDL_SCANCODE_UP])
        // {
        //     std::cout << "up key pressed! clock: " << getTime() << std::endl;
        // }

        window.clear();
        window.color(150, 150, 150, 255);
        switch (state)
        {
        case STATE_START:
        {
            float passed = clock() - timer;
            if (passed < 3000)
            {
                float passed = clock() - timer;
                Uint8 alpha = passed/2000 * 255;
                if (passed >= 2000) // Overflow
                {
                    alpha = 255;
                }
                window.renderTextCenter(0, 0, "Seacher", getFont(100, true), {0, 0, 0, alpha}, true);
            }
            if (passed >= 3000)
            {
                Uint8 alpha = (2000 - (passed - 3000))/2000 * 255;
                if (passed >= 5000) // Overflow
                {
                    alpha = 0;
                }
                window.renderTextCenter(0, 0, "Seacher", getFont(100, true), {0, 0, 0, alpha}, true);
            }
            if (passed >= 5000)
            {
                state = STATE_WAIT_KEY_START;
            }
            break;
        }

        case STATE_WAIT_KEY_START:
            window.renderTextCenter(0, -150, "Seacher", getFont(100, true), {0, 0, 0, 255}, true);
            window.renderTextCenter(0, 150, "Press Any Key to Start", getFont(50, true), {0, 0, 0, 255}, true);
            if (event.key.state == SDL_KEYDOWN || leftMouseDown)
            {
                leftMouseDown = false;
                state = STATE_PREAPARE;
                timer = clock();
            }
            break;

        case STATE_PREAPARE:
        {
            for (Desk loopDesk : desks)
            {
                window.render(*(loopDesk.getEntity()));
            }
            for (Entity entity : allEntity)
            {
                window.render(entity);
            }
            for (Desk loopDesk : desks)
            {
                if (loopDesk.getName() != "")
                {
                    Vector2f pos = loopDesk.getCoord();
                    window.renderText(pos.x + deskSize.x/2, pos.y + deskSize.y/2 + 14, loopDesk.getName().c_str(), fontB28, {0, 0, 0, 255});
                }
            }
            break;
        }
        
        default:
            break;
        }
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
                leftMouseDown = true;
            }
            else if (event.button.button == SDL_BUTTON_RIGHT)
            {
                rightMouseDown = true;
            }
            break;
        
        case SDL_MOUSEBUTTONUP:
            if (event.button.button == SDL_BUTTON_LEFT)
            {
                leftMouseDown = false;
            }
            else if (event.button.button == SDL_BUTTON_RIGHT)
            {
                rightMouseDown = false;
            }
            break;
        
        default:
            break;
        }
    }
}