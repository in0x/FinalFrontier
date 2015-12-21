#pragma once
#include "stdafx.h"
#include "RigidBodyComponent.h"
#include "Component.h"
#include "NLTmxMap.h"

//Holds the rigid body + AABB
class PhysicsComponent : public Component {
public:
	PhysicsComponent(NLTmxMapObject tileMapObject);
	PhysicsComponent(FloatRect aabb, std::shared_ptr<RigidBodyComponent> body) : pRigidBody(body), aabb(aabb) {}
	~PhysicsComponent();
	void applyImpulse(sf::Vector2f);
	std::shared_ptr<RigidBodyComponent> pRigidBody;
	FloatRect aabb;
	virtual void update(GameEvent) override;
	void setPosition(sf::Vector2f);
	void update(float fTimeDeltaSeconds);
	void move(sf::Vector2f);
private:

};