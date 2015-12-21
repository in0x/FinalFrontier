#pragma once
#include "stdafx.h"
#include "Component.h"
#include "RenderComponent.h"

class LayerComponent : public sf::Drawable { 
public:
	LayerComponent();
	~LayerComponent();
	std::vector<RenderComponent*> tiles = std::vector<RenderComponent*>(); 
	void setView(Vector2f vPos, Vector2f vSize);
protected:	
	virtual void draw(RenderTarget & target, RenderStates states) const;
};