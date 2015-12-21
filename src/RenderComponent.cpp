#pragma once
#include "stdafx.h"
#include "RenderComponent.h"



RenderComponent::RenderComponent(SpriteComponent * sprite)
{
	spriteComponent = sprite;
}

RenderComponent::RenderComponent(std::string filename, SpriteType type)
{
	spriteComponent = new SpriteComponent(filename, type);
}

void RenderComponent::update(GameEvent e)
{
	//std::cout << "DeltaTime:" << e.deltaTime << std::endl;
	/*switch (e.type)
	{
	case EventType::moveUp:
		move(0.f, -e.deltaTime*1000);
		break;
	case EventType::moveDown:
		move(0.f, e.deltaTime* 1000);
		break;
	case EventType::moveLeft:
		move(-e.deltaTime* 1000, 0.f);
		break;
	case EventType::moveRight:
		move(e.deltaTime* 1000, 0.f);
		break;

	}*/
	if (e.type == EventType::physicsMove)
		setPosition(e.movement.x, e.movement.y);
}

void RenderComponent::setSpriteComponent(SpriteComponent*  sprite)
{
	spriteComponent = sprite;
}

void RenderComponent::setPosition(float x, float y)
{
	/*if (spriteComponent->type == SpriteType::PlayerSprite)
	{
		Vector2f valid = returnValid(x, y);
		spriteComponent->setPosition(valid.x, valid.y);
	}
	else*/
		spriteComponent->setPosition(x, y);
}

void RenderComponent::move(float x, float y)
{
	/*if (spriteComponent->type == SpriteType::PlayerSprite)
	{
		Vector2f valid = spriteComponent->getPosition();
		valid = returnValid(valid.x+x, valid.y+y);
		spriteComponent->setPosition(valid.x, valid.y);
	}
	else*/
		spriteComponent->move(x, y);
}

void RenderComponent::setView(Vector2f vPos, Vector2f vSize)
{
	viewPos = vPos;
	viewSize = vSize;
	/*if (spriteComponent->type == SpriteType::PlayerSprite)
	{
	Vector2f valid = spriteComponent->getPosition();
	valid = returnValid(valid.x, valid.y);
	spriteComponent->setPosition(valid.x, valid.y);
	}
	else if (spriteComponent->type == SpriteType::ProjectileSprite)
	{
		Vector2f valid = spriteComponent->getPosition();
		valid = returnValid(valid.x, valid.y);
		spriteComponent->setPosition(valid.x, valid.y);
	}*/
}


void RenderComponent::draw(RenderTarget & target, RenderStates states) const
{
	if(isVisible())
		spriteComponent->draw(target, states);
}

bool RenderComponent::isVisible() const
{
	if (spriteComponent->type == BackgroundSprite)
		return true;
	Vector2f pos = spriteComponent->getPosition();
	Vector2f size = spriteComponent->getSize();
	float x = spriteComponent->getPosition().x;
	float y = spriteComponent->getPosition().y;
	if (pos.x + size.x<viewPos.x - viewSize.x / 2  || pos.x - size.x>viewPos.x + viewSize.x / 2 || pos.y + size.y<viewPos.y - viewSize.y / 2 || pos.y -size.y>viewPos.y + viewSize.y / 2)
		return false;
	else
		return true;
}

bool RenderComponent::checkValid(float x, float y)
{
	if(x<viewPos.x-viewSize.x/2|| x>viewPos.x + viewSize.x / 2 || y<viewPos.y - viewSize.y / 2 || y>viewPos.y + viewSize.y / 2)
		return false;
	else
		return true;
}

Vector2f RenderComponent::returnValid(float x, float y)
{
	Vector2f output = Vector2f(x, y);
	if (x < viewPos.x - viewSize.x / 2)
		output.x = viewPos.x - viewSize.x / 2;
	else
		if (x > viewPos.x + viewSize.x / 2.5)
			output.x = viewPos.x + viewSize.x / 2.5;
	if(y<viewPos.y - viewSize.y / 2)
		output.y = viewPos.y - viewSize.y / 2;
	else
		if (y>viewPos.y + viewSize.y / 2.5)
			output.y = viewPos.y + viewSize.y / 2.5;
	return output;
}

RenderComponent::~RenderComponent()
{
	delete spriteComponent;
}
