#pragma once
#include <SDL2/SDL.h>
#include "Entity.hpp"
#include <iostream>

extern SDL_Texture* deskTexture;
extern SDL_Texture* noUseDeskTexture;

class Desk
{
private:
    Entity entity;
    Vector2f coord;
    std::string name;
    bool disabled;
public:
    Desk();
    Desk(Vector2f p_coord);
    Desk(Vector2f p_coord, std::string p_name, bool p_disabled);
    Desk(Vector2f p_coord, Entity p_entity);
    Entity* getEntity();
    Vector2f getJustCoord();
    Vector2f getCoord();
    std::string getName();
    void setName(std::string p_name);
    bool isDisabled();
    void disable();
    void enable();
};

Desk::Desk()
{}

Desk::Desk(Vector2f p_coord)
: coord(p_coord)
, disabled(false)
{
    name = "";
}

Desk::Desk(Vector2f p_coord, std::string p_name, bool p_disabled)
: Desk(p_coord)
{
    name = p_name;
    disabled = p_disabled;
}

Desk::Desk(Vector2f p_coord, Entity p_entity)
: Desk(p_coord)
{
    entity = p_entity;
    entity.setPosition(getCoord());
}

Entity* Desk::getEntity()
{
    return &entity;
}

Vector2f Desk::getJustCoord()
{
    return coord;
}

Vector2f Desk::getCoord()
{
    Vector2f size = entity.getSize();

    return Vector2f(
        (size.x + 60) * (coord.x-1) + 26, 
        (size.y - 13) * (coord.y-1) + 10
    );
}

std::string Desk::getName()
{
    return name;
}

void Desk::setName(std::string p_name)
{
    name = p_name;
}

bool Desk::isDisabled()
{
    return disabled;
}

void Desk::disable()
{
    disabled = true;
    entity.setTexture(noUseDeskTexture);
    entity.setRotate(180);
}

void Desk::enable()
{
    disabled = false;
    entity.setTexture(deskTexture);
    entity.setRotate(0);
}