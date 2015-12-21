#pragma once
#include "stdafx.h"
#include "SpriteComponent.h"


SpriteComponent::SpriteComponent(Sprite* sp)
{
	sprite = *sp;
}

SpriteComponent::SpriteComponent(Sprite* sp, SpriteType type)
{
	sprite = *sp;
	this->type = type;
}


SpriteComponent::SpriteComponent(std::string filename, SpriteType type)
{
	this->type = type;
	if (type == SpriteType::BackgroundSprite)
	{
		tex.loadFromFile("../assets/" + filename);
		tex.setRepeated(true);
		tex.setSmooth(true);
		sprite = Sprite(tex);
		sprite.setTextureRect(IntRect(0, 0, 3647 * 100, tex.getSize().y));
		sprite.setPosition(0, 0);
	}
	else	//not actually called anymore, new player/tiles use above constructor
		if (type == SpriteType::PlayerSprite || type == SpriteType::ProjectileSprite)
		{
			img.loadFromFile("../assets/" + filename);
			img.createMaskFromColor(Color(0, 0, 0));
			tex.loadFromImage(img);
			tex.setSmooth(false);
			sprite = Sprite(tex);
			sprite.setTextureRect(IntRect(49, 25, 23, 23));
			sprite.setScale(3.f, 3.f);
			sprite.setPosition(0, 0);
		}
	
}


SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::draw(RenderTarget & target, RenderStates states)
{
	target.draw(sprite, states);
}

void SpriteComponent::setPosition(float x, float y)
{
	pos.x = x;
	pos.y = y;
	sprite.setPosition(pos);
}

Vector2f SpriteComponent::getPosition()
{
	return pos;
}

void SpriteComponent::move(float x, float y)
{
	pos.x += x;
	pos.y += y;
	sprite.setPosition(pos);
}

Vector2f SpriteComponent::getSize()
{
	return Vector2f(sprite.getGlobalBounds().width, sprite.getGlobalBounds().height);
}