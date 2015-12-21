#include "stdafx.h"
#include "RigidBodyComponent.h"

RigidBodyComponent::RigidBodyComponent(NLTmxMapObject tileMapObject) {
	position = { (float)tileMapObject.x, (float)tileMapObject.y };

	//Get body mass from tilemap properties
	for (auto& prop : tileMapObject.properties) {
		std::string key = prop.name;
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);

		if (key == "mass")
			mass = stof(prop.value);
	}

	invMass = (mass == 0.0f) ? 0.0f : 1.0f / mass;
}

void RigidBodyComponent::applyImpulse(sf::Vector2f impulse) {
	impulses.push_back(impulse);
}

Vector2f RigidBodyComponent::getPosition() { return position; };

float RigidBodyComponent::getMass()
{
	return mass;
}
float RigidBodyComponent::getInvMass()
{
	return invMass;
}


Vector2f RigidBodyComponent::getVelocity()
{
	return velocity;
}


Vector2f RigidBodyComponent::update(float fTimeDeltaSeconds) {


	Vector2f appliedForces{};
	for (const auto& f : forces)
		appliedForces += f;

	for (const auto& i : impulses)
		appliedForces += i;
	impulses.clear();

	//
	acceleration = appliedForces * invMass;
	velocity += acceleration * fTimeDeltaSeconds;
	velocity = velocity *  (1 - 0.9f*fTimeDeltaSeconds); //< simple "friction"
	position += velocity * fTimeDeltaSeconds;

	return position;
}

void RigidBodyComponent::setPosition(sf::Vector2f pos) {
	position = pos;
}

void RigidBodyComponent::move(sf::Vector2f pos) {
	position += pos;
}