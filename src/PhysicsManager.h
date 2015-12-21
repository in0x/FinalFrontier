#pragma once
#include "stdafx.h"
#include "PhysicsComponent.h"
#include "Event.h"

struct Manifold
{
	PhysicsComponent* body1;
	PhysicsComponent* body2;

	float penetration;
	Vector2f normal;
};


class PhysicsManager : public Observable, public Observer {
public:
	void update(float fTimeDeltaSeconds);
	void addComponent(shared_ptr<PhysicsComponent>);
	void printComponents();
	void clear();
	virtual void update(GameEvent) override;
	~PhysicsManager();
private:
	void handleCollisions();
	void resolveCollisions();
	void findCollisions();
	bool AABBvsAABB(const FloatRect& a, const FloatRect& b, Vector2f& normal, float& penetration);

	std::vector<shared_ptr<PhysicsComponent>> pBodies;
	std::vector<Manifold> manifolds;
};