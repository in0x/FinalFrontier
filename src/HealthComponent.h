#pragma once
#include "stdafx.h"
#include "Component.h"

class HealthComponent : public Component {
public:
	HealthComponent(double h) : health(h) {}
	virtual void update(GameEvent) override;
private:
	double health;
	sf::Clock damageCooldown{};
};