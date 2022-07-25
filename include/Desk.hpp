#pragma once
#include "Entity.hpp"
#include <iostream>

class Desk
{
private:
    Entity entity;
    Vector2f coord;
    std::string name;
public:
    Desk(Entity p_entity, Vector2f p_coord);
    Entity* getEntity();
    Vector2f getCoord();
    std::string getName();
    void setName(std::string p_name);
};

Desk::Desk(Entity p_entity, Vector2f p_coord)
: entity(p_entity)
, coord(p_coord)
{
    entity.setPosition(getCoord());
    name = "";
}

Entity* Desk::getEntity()
{
    return &entity;
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