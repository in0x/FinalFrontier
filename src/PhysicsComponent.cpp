#pragma once
#include "stdafx.h"
#include "PhysicsComponent.h"


PhysicsComponent::PhysicsComponent(NLTmxMapObject tileMapObject) {
	pRigidBody =  std::make_shared<RigidBodyComponent>( tileMapObject );
	if (tileMapObject.type == "player") {
		aabb = FloatRect{ (float)tileMapObject.x,  (float)tileMapObject.y,  (float)tileMapObject.width*3, (float)tileMapObject.height*3 };
	}
	else
		aabb = FloatRect{ (float)tileMapObject.x,  (float)tileMapObject.y,  (float)tileMapObject.width, (float)tileMapObject.height};
}
	
PhysicsComponent::~PhysicsComponent() {}

void PhysicsComponent::applyImpulse(sf::Vector2f impulse)
{
	pRigidBody->applyImpulse(impulse);
}

void PhysicsComponent::setPosition(sf::Vector2f pos) {
	aabb.left = pos.x;
	aabb.top = pos.y;
	pRigidBody->setPosition(pos);
}

void PhysicsComponent::move(sf::Vector2f pos) {
	aabb.left += pos.x;
	aabb.top += pos.y;
	pRigidBody->move(pos);
}

void PhysicsComponent::update(float fTimeDeltaSeconds) {
	Vector2f position = pRigidBody->update(fTimeDeltaSeconds);
	aabb.left = position.x;
	aabb.top = position.y;
	updateObservers(GameEvent{EventType::physicsMove, position});
}

void PhysicsComponent::update(GameEvent e) {
	float ms = 5000;
	if (e.type == EventType::moveUp) {
		applyImpulse(Vector2f{ 0, -ms });
	}
	else if (e.type == EventType::moveDown) {
		applyImpulse(Vector2f{ 0, ms });
	}
	else if (e.type == EventType::moveRight) {
		applyImpulse(Vector2f{ ms, 0 });
	}
	else if (e.type == EventType::moveLeft) {
		applyImpulse(Vector2f{ -ms, -0 });
	}
}
