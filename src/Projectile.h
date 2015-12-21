#pragma once
#include "stdafx.h"
#include "Component.h"
#include "GameObject.h"
#include "Renderer.h"


class Projectile : public GameObject {
public:
	Projectile(std::string id, RenderComponent* r);
	~Projectile();
	bool needsDelete();
	void move(float x);
private:
	RenderComponent* rendComp;
	sf::Clock lifeTime;
	float timeElapsedSeconds = 0;
};

class ProjectileComponent : public Component {
public:
	ProjectileComponent(Renderer& r) : rend(r) {}
	void update(GameEvent e) override;
private:
	static int projectileCounter;
	Renderer& rend;
	std::vector<Projectile*> projectiles;
	sf::Clock spawnClock = sf::Clock();
	float timeSinceLastSpawn;
};
