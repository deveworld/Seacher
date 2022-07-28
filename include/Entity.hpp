#pragma once
#include "Math.hpp"

class Entity
{
private:
    SDL_Rect frame;
    Vector2f position; 
    SDL_Texture* texture;
    Vector2f scale;
    double angle;
    SDL_RendererFlip flip;
public:
    Entity();
    Entity(Vector2f p_position, SDL_Texture* p_texture);
    Entity(Vector2f p_position, SDL_Texture* p_texture, Vector2f p_scale);
    Entity(Vector2f p_position, SDL_Texture* p_texture, double p_angle);
    Entity(Vector2f p_position, SDL_Texture* p_texture, Vector2f p_scale, double p_angle);
    SDL_Rect getFrame();
    Vector2f getPosition();
    SDL_Texture* getTexture();
    Vector2f getScale();
    SDL_RendererFlip getFlip();
    double getRotate();
    Vector2f getSize();
    void center();
    void move(float x, float y);
    void setPosition(float x, float y);
    void setPosition(Vector2f p_position);
    void setTexture(SDL_Texture* p_texture);
    void setScale(float x, float y);
    void rotate(double p_angle);
    void setRotate(double p_angle);
    void setFlip(SDL_RendererFlip p_flip);
    void flipHorizontal();
    void flipVertical();
};

Entity::Entity()
{}

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

Entity::Entity(Vector2f p_position, SDL_Texture* p_texture, double p_angle)
: Entity(p_position, p_texture)
{
    angle = p_angle;
}

Entity::Entity(Vector2f p_position, SDL_Texture* p_texture, Vector2f p_scale, double p_angle)
: Entity(p_position, p_texture, p_scale)
{
    angle = p_angle;
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

double Entity::getRotate()
{
	return angle;
}

Vector2f Entity::getSize()
{
    SDL_Rect src = getFrame();
    return Vector2f(src.w * scale.x, src.h * scale.y);
}

SDL_RendererFlip Entity::getFlip()
{
    return flip;
}

void Entity::center()
{
    SDL_Rect src = getFrame();

    position.x = SCREEN_X/2 - (src.w * scale.x)/2;
	position.y = SCREEN_Y/2 - (src.h * scale.y)/2;
}

void Entity::move(float x, float y)
{
	position.x += x;
	position.y += y;
}

void Entity::setPosition(float x, float y)
{
	position.x = x;
	position.y = y;
}

void Entity::setPosition(Vector2f p_position)
{
    position = p_position;
}

void Entity::setTexture(SDL_Texture* p_texture)
{
    texture = p_texture;
}

void Entity::setScale(float w, float h)
{
	scale.x = w;
	scale.y = h;
}

void Entity::rotate(double p_angle)
{
	angle += p_angle;
    while (abs(angle) > 360)
    {
        if (angle > 0)
        {
            angle -= 360;
        }
        else if (angle < 0)
        {
            angle += 360;
        }
    }
}

void Entity::setRotate(double p_angle)
{
	angle = p_angle;
    while (abs(angle) > 360)
    {
        if (angle > 0)
        {
            angle -= 360;
        }
        else if (angle < 0)
        {
            angle += 360;
        }
    }
}

void Entity::setFlip(SDL_RendererFlip p_flip)
{
    flip = p_flip;
}

void Entity::flipHorizontal()
{
    if (flip == SDL_RendererFlip::SDL_FLIP_HORIZONTAL)
    {
        flip = SDL_RendererFlip::SDL_FLIP_NONE;
    }
    else {
        flip = SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
    }
}

void Entity::flipVertical()
{
    if (flip == SDL_RendererFlip::SDL_FLIP_VERTICAL)
    {
        flip = SDL_RendererFlip::SDL_FLIP_NONE;
    }
    else {
        flip = SDL_RendererFlip::SDL_FLIP_VERTICAL;
    }
}