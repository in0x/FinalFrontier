#include "stdafx.h"
#include "PhysicsManager.h"


PhysicsManager::~PhysicsManager() {
	pBodies.clear();
}

void PhysicsManager::clear() {
	pBodies.clear();
}

void PhysicsManager::addComponent(shared_ptr<PhysicsComponent> comp) {
	pBodies.push_back(comp);
}

void PhysicsManager::printComponents() {
	int bods = 0;
	for (auto& comp : pBodies) {
		std::cout << bods << "Component" << std::endl;
		bods++;
	}
}

void PhysicsManager::update(GameEvent e) {
	
}

void PhysicsManager::update(float fTimeDeltaSeconds) {
	handleCollisions();

	// physical simulation loop
	for (auto& body : pBodies)
	{
		body->update(fTimeDeltaSeconds);
	}
}

void PhysicsManager::handleCollisions() {
	manifolds.clear();

	findCollisions();
	resolveCollisions();
}

void PhysicsManager::resolveCollisions() {
	for (auto man : manifolds)
	{
		man.body1->updateObservers(GameEvent{EventType::damageTaken, 1});
		man.body2->updateObservers(GameEvent{EventType::damageTaken, 1});

		// Calculate relative velocity
		Vector2f rv = man.body1->pRigidBody->getVelocity() - man.body2->pRigidBody->getVelocity();

		// Calculate relative velocity in terms of the normal direction (length through vector projection)
		float velAlongNormal = rv.x * man.normal.x + rv.y * man.normal.y;

		// Do not resolve if velocities are separating
		if (velAlongNormal > 0)
			return;

		bool restitutionOn = true;
		if (restitutionOn)
		{
			// Calculate impulse scalar
			const float e = 1.0f; //< all objects are bouncy
			float j = -(1 + e) * velAlongNormal;
			j /= man.body1->pRigidBody->getInvMass() + man.body2->pRigidBody->getInvMass();

			// Apply impulse
			Vector2f impulse = j * man.normal;

			// TODO: add mass and restitution
			//man.body1->velocity += impulse * man.body1->invMass;
			//man.body2->velocity -= impulse * man.body2->invMass;
			man.body1->applyImpulse(impulse);
			man.body2->applyImpulse(-impulse);

			//err() << "velAlongNormal " << velAlongNormal << " impulse " << impulse.x << " " << impulse.y << endl;
		}
		else
		{
			// Apply impulse
			Vector2f impulse = velAlongNormal * man.normal;

			man.body1->applyImpulse(-0.5f * impulse);
			man.body2->applyImpulse(0.5f * impulse);
		}

		// Positional correction
		bool positionalCorrection = true;
		if (positionalCorrection)
		{
			const float percent = 0.2f; // usually 20% to 80%
			const float slop = 0.01f; // usually 0.01 to 0.1
			Vector2f correction = std::max(man.penetration - slop, 0.0f) /
				(man.body1->pRigidBody->getInvMass() + man.body2->pRigidBody->getInvMass()) * percent * man.normal;
			// Apply directly to position
			man.body1->setPosition(man.body1->pRigidBody->getPosition() + man.body1->pRigidBody->getInvMass() * correction);
			man.body2->setPosition(man.body2->pRigidBody->getPosition() - man.body2->pRigidBody->getInvMass() * correction);
		}
	}
}

void PhysicsManager::findCollisions() {
	for (int i = 0; i < pBodies.size(); ++i)
	{
		auto& body1 = pBodies[i];
		for (int j = i+1; j < pBodies.size(); ++j)
		{
			auto& body2 = pBodies[j];

			if (body1->pRigidBody->getMass() == 0 && body2->pRigidBody->getMass() == 0)
			{
				// if both object don't have a mass or body is the same skip
				continue;
			}

			Vector2f normal;
			float penetration;


			if (AABBvsAABB(body1->aabb,	body2->aabb, normal, penetration))
			{

				Manifold manifold;
				manifold.body1 = body1.get();
				manifold.body2 = body2.get();
				manifold.normal = normal;
				manifold.penetration = penetration;

				manifolds.push_back(manifold);
			}
		}
	}
}

bool PhysicsManager::AABBvsAABB(const FloatRect& a, const FloatRect& b, Vector2f& normal, float& penetration) {
	auto getCenter = [](const sf::FloatRect& rect) -> Vector2f
	{
		return Vector2f(rect.left, rect.top) + 0.5f * Vector2f(rect.width, rect.height);
	};

	Vector2f n = getCenter(b) - getCenter(a);		// Vector from A to B
	float a_extent = a.width * 0.5f;			    // Calculate half extents along x axis
	float b_extent = b.width * 0.5f;
	float x_overlap = a_extent + b_extent - abs(n.x);		// Calculate overlap on x axis
															// SAT test on x axis
	if (x_overlap > 0) {
		float a_extent = a.height * 0.5f;		// Calculate half extents along y axis
		float b_extent = b.height * 0.5f;
		float y_overlap = a_extent + b_extent - abs(n.y);	// Calculate overlap on y axis

															// SAT test on y axis
		if (y_overlap > 0) {
			// Find out which axis is axis of least penetration
			if (x_overlap < y_overlap) {
				// Point towards B knowing that n points from A to B
				if (n.x < 0)
					normal = Vector2f(1.0f, 0.0f);
				else
					normal = Vector2f(-1.0f, 0.0f);
				penetration = x_overlap;
				return true;
			}
			else {
				// Point towards B knowing that n points from A to B
				if (n.y < 0)
					normal = Vector2f(0, 1);
				else
					normal = Vector2f(0, -1);
				penetration = y_overlap;
				return true;
			}
		}
	}
	return false;
}
