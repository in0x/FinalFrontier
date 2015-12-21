#pragma once
#include "stdafx.h"
#include "Projectile.h"

void ProjectileComponent::update(GameEvent e) {
	
	timeSinceLastSpawn += spawnClock.getElapsedTime().asSeconds();

	for (auto& projectile : projectiles) {
		if (projectile != NULL) {
			projectile->move(10.f);
			if (projectile->needsDelete())
			{
				delete projectile;
			}
		}
	}

	/*std::vector<Projectile*>::iterator it;
	for (it = projectiles.begin(); it != projectiles.end(); it++) {
		if ((*it)->needsDelete())
		{
			delete (*it);
			it = projectiles.erase(it);
		}
	}*/

	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && timeSinceLastSpawn > 10) {
		timeSinceLastSpawn = 0;
		RenderComponent* rendComp = new RenderComponent("ShipGun2.bmp", SpriteType::ProjectileSprite);
		projectileCounter++;
		Projectile* pro = new Projectile("projectile" + projectileCounter, rendComp);
		projectiles.push_back(pro);
//		rend.registerRenderComponent(rendComp);

	}
}

int ProjectileComponent::projectileCounter = 0;

Projectile::Projectile(std::string id, RenderComponent* r) : GameObject(id) {
	rendComp = r;
	lifeTime = sf::Clock();
}

Projectile::~Projectile() {
	delete rendComp;
}

void Projectile::move(float x) {
	rendComp->move(x, 0);
}

bool Projectile::needsDelete() {
	timeElapsedSeconds += lifeTime.getElapsedTime().asSeconds();
	std::cout << timeElapsedSeconds << std::endl;
	return timeElapsedSeconds >= 5;
}