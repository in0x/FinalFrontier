#include "stdafx.h"
#include "GameObject.h"

void GameObject::addComponent(Component* c) {
	components.push_back(c);
	c->addObserver(this);
	addObserver(c);
}

void GameObject::removeComponent(Component* c) {

	std::vector<Component*>::iterator it;

	for (it = components.begin(); it != components.end(); it++) {
		if (*it == c)
			break;
	}

	if (*it != c) {
		return;
	}

	components.erase(it);
}

void GameObject::update(GameEvent e) {
	eventsSinceUpdate.push_back(e);
}

GameEvent GameObject::updateComponents(float deltaTime) {

	std::vector<GameEvent> events = eventsSinceUpdate;
	eventsSinceUpdate.clear();

	for (auto e : events) {
		if (e.type == EventType::unitDied)
			return GameEvent{ EventType::unitDied };
		e.deltaTime = deltaTime;
		updateObservers(e);
	}

	return GameEvent{ EventType::none };
}

GameObject::~GameObject() {
	std::cout << "~GameObject() " + id << std::endl;
	for (auto& component : components){
		if (component == nullptr)
			continue;

		RenderComponent* c = dynamic_cast<RenderComponent*>(component);
		if (c != nullptr)
			continue;

//		delete component;
		component = nullptr;
	}
}