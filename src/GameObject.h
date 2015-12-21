#pragma once
#include "stdafx.h"
#include "Component.h"
#include "InputComponent.h"
#include "RenderComponent.h"
#include "Event.h"

class GameObject : public Observer, public Observable{
public:
	GameObject(std::string id) : id(id) {}
	~GameObject();
	std::string id;
	void addComponent(Component* c);
	void removeComponent(Component* c);
	GameEvent updateComponents(float deltaTime); // This functions now returns GameEvents so we can tell the mainstate that the player died
	void update(GameEvent e) override;
private:
	std::vector<GameEvent> eventsSinceUpdate = std::vector<GameEvent>();
	std::vector<Component*> components = std::vector<Component*>(); 
};

