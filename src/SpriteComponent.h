#pragma once
#include "stdafx.h"
#include "Component.h"
enum SpriteType
{
	PlayerSprite = 0,
	ProjectileSprite = 1,
	BackgroundSprite = 2,
	TileSprite = 3
};

class SpriteComponent :
	public Component
{
public:
	
	SpriteComponent::SpriteComponent(Sprite* sp);
	SpriteType type;
	SpriteComponent(Sprite* sp, SpriteType type);
	SpriteComponent::SpriteComponent(std::string filename, SpriteType type);
	~SpriteComponent();
	virtual void draw(RenderTarget &target, RenderStates states);
	virtual void update(GameEvent e) override {};
	void setPosition(float x, float y);
	Vector2f getPosition();
	void move(float x, float y);
	Vector2f getSize();
private:
	Sprite sprite;
	Texture tex;
	Image img;
	Vector2f pos;
};

