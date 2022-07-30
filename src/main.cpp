#include "imgui.h"
#include "imgui_stdlib.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <iostream>
#include <vector>
#include <algorithm>

const int SCREEN_X = 1200;
const int SCREEN_Y = 900;

const int STATE_START                   = 001;
const int STATE_WAIT_KEY_START          = 002;
const int STATE_PREPARE_NO_DESK         = 003;
const int STATE_PREPARE_NAME            = 005;
const int STATE_PREPARE_NAME_CHECK      = 006;
const int STATE_PREPARE_SEPARATE_NAME   = 007;
const int STATE_PREPARE_FRONT_NAME      = 010;
const int STATE_ARRANGE                 = 011;

#include "RenderWindow.hpp"
#include "Desk.hpp"
#include "Arranger.hpp"

bool init();
void eventHandle();
TTF_Font* getFont(int size, bool bold = false);
void renderEssential();

bool running = true;
int state = STATE_START;
bool nameWrong = false;
clock_t timer = clock();

int row, column;
std::string rawNames = "";
std::string namesCheck = "";
std::string rawSeparateNames = "";
std::string rawFrontNames = "";
std::vector<std::string> names;
std::vector<std::string> separateNames;
std::vector<std::string> frontNames;

bool leftMouseDown = false;
bool rightMouseDown = false;

const Uint8 *key = SDL_GetKeyboardState(NULL);

SDL_Event event;
bool inited = init();

RenderWindow window("Seacher", SCREEN_X, SCREEN_Y);
Arranger* arranger = nullptr;

SDL_Texture* blackboardTexture      = window.loadTexture("resources/blackboard.png");
SDL_Texture* deskTexture            = window.loadTexture("resources/desk.png");
SDL_Texture* noUseDeskTexture       = window.loadTexture("resources/noUseDesk.png");
SDL_Texture* teacherDeskTexture     = window.loadTexture("resources/teacherDesk.png");

std::vector<Entity> allEntity;
std::vector<Desk> desks;

ImVec4 backgroundColor = ImVec4(0.79215686274, 0.6431372549, 0.44705882352, 1);

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

    Entity blackboard     = Entity(Vector2f(0, 0), blackboardTexture, Vector2f(0.45, 0.2));
    Entity desk           = Entity(Vector2f(0, 0), deskTexture, Vector2f(0.13, 0.13));
    Entity teacherDesk    = Entity(Vector2f(0, 0), teacherDeskTexture, Vector2f(0.13, 0.13));

    blackboard.center();
    blackboard.move(0, 438);
    blackboard.flipHorizontal();

    teacherDesk.center();
    teacherDesk.move(0, 350);

    desk.flipHorizontal();

    for (size_t i = 1; i <= 6; i++)
    {
        for (size_t j = 1; j <= 6; j++)
        {
            Desk newDesk = Desk(Vector2f(i, j), desk);
            desks.push_back(newDesk);
        }
    }

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

        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        window.clear();
        window.color(backgroundColor);
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
        {
            window.renderTextCenter(0, -150, "Seacher", getFont(100, true), {0, 0, 0, 255}, true);
            window.renderTextCenter(0, 150, "Click Anywhere to Start", getFont(50, true), {0, 0, 0, 255}, true);
            if (leftMouseDown)
            {
                leftMouseDown = false;
                state = STATE_PREPARE_NO_DESK;
                timer = clock();
            }
            break;
        }

        case STATE_PREPARE_NO_DESK:
        {
            renderEssential();

            if (leftMouseDown && !window.getIO()->WantCaptureMouse)
            {
                leftMouseDown = false;
                int noUseDeskX = 0;
                int noUseDeskY = 0;
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                switch (mouseX)
                {
                case 26 ... 168: // 1
                    noUseDeskX = 1;
                    break;

                case 228 ... 369: // 2
                    noUseDeskX = 2;
                    break;
                
                case 429 ... 571: // 3
                    noUseDeskX = 3;
                    break;

                case 630 ... 772: // 4
                    noUseDeskX = 4;
                    break;
                
                case 832 ... 974: // 5
                    noUseDeskX = 5;
                    break;

                case 1034 ... 1175: // 6
                    noUseDeskX = 6;
                    break;
                
                default:
                    break;
                }
                switch (mouseY)
                {
                case 10 ... 133: // 1
                    noUseDeskY = 1;
                    break;

                case 134 ... 256: // 2
                    noUseDeskY = 2;
                    break;

                case 257 ... 380: // 3
                    noUseDeskY = 3;
                    break;

                case 381 ... 504: // 4
                    noUseDeskY = 4;
                    break;

                case 505 ... 627: // 5
                    noUseDeskY = 5;
                    break;

                case 628 ... 751: // 6
                    noUseDeskY = 6;
                    break;
                
                default:
                    break;
                }
                if (noUseDeskX != 0 && noUseDeskY != 0)
                {
                    if (desks[((noUseDeskX)-1)*6 + noUseDeskY - 1].isDisabled())
                    {
                        desks[((noUseDeskX)-1)*6 + noUseDeskY - 1].enable();
                    }
                    else {
                        desks[((noUseDeskX)-1)*6 + noUseDeskY - 1].disable();
                    }
                }
            }

            {
                ImGui::SetNextWindowSize(ImVec2(300, 100), ImGuiCond_Once);
                ImGui::SetNextWindowPos(ImVec2(SCREEN_X/2, SCREEN_Y/2), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
                ImGui::Begin("Students Settings-1");
                ImGui::Text("Click none using desk");
                if (ImGui::Button("Done", ImVec2(50, 30)))
                {
                    state = STATE_PREPARE_NAME;
                }
                ImGui::End();
            }
            
            break;
        }

        case STATE_PREPARE_NAME:
        {
            renderEssential();

            {
                ImGui::SetNextWindowSize(ImVec2(316, 400), ImGuiCond_Once);
                ImGui::SetNextWindowPos(ImVec2(SCREEN_X/2, SCREEN_Y/2), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
                ImGui::Begin("Students Settings-2");
                ImGui::Text("Enter student names(Separating by lines)");
                if (nameWrong)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255,50,50,255));
                    ImGui::Text("The number of seats and names do not match");
                    ImGui::PopStyleColor();
                }
                ImGui::InputTextMultiline(
                    "##Names", 
                    &rawNames,
                    ImVec2(300, 300), 
                    ImGuiInputTextFlags_CharsNoBlank
                );
                if (ImGui::Button("Done", ImVec2(50, 30)))
                {
                    std::string buffer = rawNames;
                    size_t pos = 0;
                    std::string token;
                    std::string delimiter = "\n";
                    do
                    {
                        pos = buffer.find(delimiter);

                        if (pos == std::string::npos)
                        {
                            token = buffer;
                        }
                        else
                        {
                            token = buffer.substr(0, pos);
                        }

                        if (token[token.size() -1] == '\r')
                        {
                            token = token.substr(0, token.size() -1);
                        }

                        names.push_back(token);
                        buffer.erase(0, pos + delimiter.length());
                    } while (pos != std::string::npos);
                    size_t deskCount = 0;
                    for (Desk desk : desks)
                    {
                        if (!desk.isDisabled())
                        {
                            deskCount++;
                        }
                    }
                    if (deskCount != names.size())
                    {
                        names.clear();
                        nameWrong = true;
                        ImGui::End();
                        break;
                    }
                    
                    size_t i = 1; 
                    namesCheck = "";
                    for (std::string name : names)
                    {
                        namesCheck += std::to_string(i) + ": " + name + "\n";
                        i++;
                    }
                    state = STATE_PREPARE_NAME_CHECK;
                }
                ImGui::End();
            }
            break;
        }

        case STATE_PREPARE_NAME_CHECK:
        {
            renderEssential();

            {
                ImGui::SetNextWindowSize(ImVec2(316, 400), ImGuiCond_Once);
                ImGui::SetNextWindowPos(ImVec2(SCREEN_X/2, SCREEN_Y/2), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
                ImGui::Begin("Students Settings-3");
                ImGui::Text("Student names checking");
                ImGui::InputTextMultiline(
                    "##NamesCheck", 
                    &namesCheck,
                    ImVec2(300, 300), 
                    ImGuiInputTextFlags_ReadOnly
                );
                if (ImGui::Button("Ok", ImVec2(50, 30)))
                {
                    rawNames = "";
                    namesCheck = "";
                    int i = 0;
                    for (Desk& desk : desks)
                    {
                        if (!desk.isDisabled())
                        {
                            desk.setName(names[i]);
                            i++;
                        }
                    }
                    state = STATE_PREPARE_SEPARATE_NAME;
                }
                ImGui::SameLine(0.0F, 15.0F);
                if (ImGui::Button("Cancel", ImVec2(70, 30)))
                {
                    namesCheck = "";
                    names.clear();
                    state = STATE_PREPARE_NAME;
                }
                ImGui::End();
            }

            break;
        }

        case STATE_PREPARE_SEPARATE_NAME:
        {
            renderEssential();

            {
                ImGui::SetNextWindowSize(ImVec2(316, 400), ImGuiCond_Once);
                ImGui::SetNextWindowPos(ImVec2(SCREEN_X/2, SCREEN_Y/2), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
                ImGui::Begin("Students Settings-4");
                ImGui::Text("Enter student separate names");
                ImGui::InputTextMultiline(
                    "##SeparateNames", 
                    &rawSeparateNames,
                    ImVec2(300, 300), 
                    ImGuiInputTextFlags_CharsNoBlank
                );
                if (ImGui::Button("Done", ImVec2(50, 30)))
                {
                    std::string buffer = rawSeparateNames;
                    size_t pos = 0;
                    std::string token;
                    std::string delimiter = "\n";
                    do
                    {
                        pos = buffer.find(delimiter);

                        if (pos == std::string::npos)
                        {
                            token = buffer;
                        }
                        else
                        {
                            token = buffer.substr(0, pos);
                        }

                        if (token[token.size() -1] == '\r')
                        {
                            token = token.substr(0, token.size() -1);
                        }

                        separateNames.push_back(token);
                        buffer.erase(0, pos + delimiter.length());
                    } while (pos != std::string::npos);

                    state = STATE_PREPARE_FRONT_NAME;
                }
                ImGui::End();
            }
            break;
        }

        case STATE_PREPARE_FRONT_NAME:
        {
            renderEssential();

            {
                ImGui::SetNextWindowSize(ImVec2(316, 400), ImGuiCond_Once);
                ImGui::SetNextWindowPos(ImVec2(SCREEN_X/2, SCREEN_Y/2), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
                ImGui::Begin("Students Settings-5");
                ImGui::Text("Enter student front names");
                ImGui::InputTextMultiline(
                    "##FrontNames", 
                    &rawFrontNames,
                    ImVec2(300, 300), 
                    ImGuiInputTextFlags_CharsNoBlank
                );
                if (ImGui::Button("Done", ImVec2(50, 30)))
                {
                    std::string buffer = rawFrontNames;
                    size_t pos = 0;
                    std::string token;
                    std::string delimiter = "\n";
                    do
                    {
                        pos = buffer.find(delimiter);

                        if (pos == std::string::npos)
                        {
                            token = buffer;
                        }
                        else
                        {
                            token = buffer.substr(0, pos);
                        }

                        if (token[token.size() -1] == '\r')
                        {
                            token = token.substr(0, token.size() -1);
                        }

                        frontNames.push_back(token);
                        buffer.erase(0, pos + delimiter.length());
                    } while (pos != std::string::npos);
                    Arranger buf = Arranger(&desks, &names, &separateNames, &frontNames);
                    arranger = &buf;

                    state = STATE_ARRANGE;
                }
                ImGui::End();
            }
            break;
        }

        case STATE_ARRANGE:
        {
            renderEssential();

            {
                ImGui::SetNextWindowSize(ImVec2(310, 100), ImGuiCond_Once);
                ImGui::SetNextWindowPos(ImVec2(SCREEN_X/2, SCREEN_Y/2), ImGuiCond_Once, ImVec2(0.5f, 0.5f));
                ImGui::Begin("Students Arrannge");
                ImGui::Text("Students arrannge");
                if (ImGui::Button("Start", ImVec2(70, 30)))
                {
                    arranger->start();
                }
                ImGui::SameLine(0.0F, 15.0F);
                if (ImGui::Button("Stop", ImVec2(70, 30)))
                {
                    arranger->stop();
                }
                ImGui::SameLine(0.0F, 15.0F);
                if (ImGui::Button("Stop & Save", ImVec2(120, 30)))
                {
                    arranger->stop();
                }
                ImGui::End();
            }

            break;
        }
        
        default:
            break;
        }
        ImGui::Render();
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        window.display();
    }

    ImGui_ImplSDLRenderer_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    window.cleanUp();
    SDL_Quit();

    return 0;
}

void renderEssential()
{
    Vector2f deskSize = desks[0].getEntity()->getSize();
    for (Desk loopDesk : desks)
    {
        window.render(*(loopDesk.getEntity()));
        if (loopDesk.getName() != "")
        {
            Vector2f pos = loopDesk.getCoord();
            window.renderText(pos.x + deskSize.x/2, pos.y + deskSize.y/2 + 14, loopDesk.getName().c_str(), fontB28, {0, 0, 0, 255});
        }
    }
    for (Entity entity : allEntity)
    {
        window.render(entity);
    }

    {
        ImGui::Begin("Window Info");
        ImGui::SetWindowSize(ImVec2(450, 100), ImGuiCond_Once);
        ImGui::SetWindowPos(ImVec2(740, 10), ImGuiCond_Once);
        ImGui::ColorEdit4("Background color", (float*)&backgroundColor);
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
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
        ImGui_ImplSDL2_ProcessEvent(&event);
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