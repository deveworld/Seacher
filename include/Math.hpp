#pragma once
#include <iostream>

struct Vector2f
{
    float x, y;

    Vector2f()
    : x(0)
    , y(0)
    {}
    
    Vector2f(float p_x, float p_y)
    : x(p_x)
    , y(p_y)
    {}
};

class DeskForCal
{
private:
    Vector2f coord;
    std::string name = "";
    bool disabled;
public:
    DeskForCal();
    DeskForCal(Vector2f p_coord, std::string p_name, bool p_disabled);
    Vector2f getCoord();
    std::string getName();
    void setName(std::string p_name);
    bool isDisabled();
};

DeskForCal::DeskForCal()
{}

DeskForCal::DeskForCal(Vector2f p_coord, std::string p_name, bool p_disabled)
: coord(p_coord)
, name(p_name)
, disabled(p_disabled)
{}

Vector2f DeskForCal::getCoord()
{
    return coord;
}

std::string DeskForCal::getName()
{
    return name;
}

void DeskForCal::setName(std::string p_name)
{
    name = p_name;
}

bool DeskForCal::isDisabled()
{
    return disabled;
}