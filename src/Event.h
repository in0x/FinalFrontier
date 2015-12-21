#pragma once

#include "stdafx.h"
#include <vector>

#include "stdafx.h"

enum class EventType {
	moveUp,
	moveDown,
	moveLeft,
	moveRight,
	shoot,
	physicsMove,
	damageTaken,
	unitDied,
	collisionOccured,
	none
};

class GameEvent {
public:
	GameEvent() {}
	GameEvent(EventType t) : type(t) {}
	GameEvent(EventType t, double d) : type(t), damage(d) {}
	GameEvent(EventType t, sf::Vector2f mv) : type(t), movement(mv) {}
	GameEvent(sf::Event* e) : pEvent(e) {}
	GameEvent(sf::Event* e, EventType t) : pEvent(e), type(t) {}
	GameEvent(sf::Event* e, EventType t, float f) : pEvent(e), type(t), deltaTime(f) {}
	sf::Event* pEvent = nullptr;

	float deltaTime = 0;
	double damage = 0;
	sf::Vector2f movement{};
	EventType type;
};


class Observer {
public:
	virtual void update(GameEvent e) {  }
};

class Observable {
public: 
	void addObserver(Observer*);
	void removeObserver(Observer*);
	void updateObservers(GameEvent e);
	std::vector<Observer*> observers;
};
