#pragma once
#include "Math.hpp"

class Entity
{
private:
    SDL_Rect frame;
    Vector2f position; 
    SDL_Texture* texture;
    Vector2f scale; 
public:
    Entity(Vector2f p_position, SDL_Texture* p_texture);
    Entity(Vector2f p_position, SDL_Texture* p_texture, Vector2f p_scale);
    SDL_Rect getFrame();
    Vector2f getPosition();
    SDL_Texture* getTexture();
    Vector2f getScale();
    void setPosition(float x, float y);
    void setScale(float x, float y);
};

Entity::Entity(Vector2f p_position, SDL_Texture* p_texture)
: position(p_position)
, texture(p_texture)
, scale(Vector2f(1, 1))
{
    frame.x = 0;
    frame.y = 0;
    SDL_QueryTexture(texture, NULL, NULL, &frame.w, &frame.h);
}

Entity::Entity(Vector2f p_position, SDL_Texture* p_texture, Vector2f p_scale)
: Entity(p_position, p_texture)
{
    scale = p_scale;
}

SDL_Rect Entity::getFrame()
{
    return frame;
}

Vector2f Entity::getPosition()
{
    return position;
}

SDL_Texture* Entity::getTexture()
{
    return texture;
}

Vector2f Entity::getScale()
{
    return scale;
}

void Entity::setPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void Entity::setScale(float w, float h)
{
	scale.x = w;
	scale.y = h;
}