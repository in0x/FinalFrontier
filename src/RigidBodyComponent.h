#pragma once
#include "stdafx.h"
#include "Component.h"
#include "NLTmxMap.h"

//Holds the Rigid body (velocitiy, forces)
class RigidBodyComponent : public Component {
public:
	RigidBodyComponent(NLTmxMapObject tileMapObject);
	RigidBodyComponent(float mass, const Vector2f& position) : mass(mass), position(position) {
		invMass = (mass == 0.0f) ? 0.0f : 1.0f / mass;
	}
	
	void applyImpulse(sf::Vector2f);
	void setPosition(sf::Vector2f);
	void move(sf::Vector2f);

	float getInvMass();
	float getMass();

	Vector2f getVelocity();
	Vector2f getPosition();
	Vector2f update(float fTimeDeltaSeconds);
	
private:
	float mass = 0;
	float invMass = 0;

	std::vector<Vector2f> forces;
	std::vector<Vector2f> impulses;

	Vector2f position{};
	Vector2f acceleration{};
	Vector2f velocity{};

};