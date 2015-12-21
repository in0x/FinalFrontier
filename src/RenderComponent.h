#pragma once
#include "Component.h"
#include "SpriteComponent.h"
class RenderComponent :
	public Component, public sf::Drawable
{
public:
	~RenderComponent();

	//RenderComponent();
	RenderComponent(SpriteComponent *sprite);
	RenderComponent(std::string filename, SpriteType type);
	virtual void update(GameEvent e) override;
	void setSpriteComponent(SpriteComponent *sprite);
	void setPosition(float x, float y);
	void move(float x, float y);
	void setView(Vector2f vPos, Vector2f vSize);
	virtual void draw(RenderTarget & target, RenderStates states) const;
private:
	SpriteComponent* spriteComponent;
	Vector2f viewPos;
	Vector2f viewSize;
	bool isVisible() const;
	bool checkValid(float x, float y);
	Vector2f returnValid(float x, float y);
};

