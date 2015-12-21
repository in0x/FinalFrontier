#include "stdafx.h"
#include "HealthComponent.h"

void HealthComponent::update(GameEvent e) {
	if (e.type == EventType::damageTaken && damageCooldown.getElapsedTime().asSeconds() > 1) {
		std::cout << "Damage" << std::endl;
		health -= e.damage;
		damageCooldown.restart();
		std::cout << "New health: " + std::to_string(health) << std::endl;
	}
	if (health <= 0)
		updateObservers(GameEvent{EventType::unitDied});
}