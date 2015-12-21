// Game.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

struct Body
{
	FloatRect shape;
	RectangleShape debugGeometry;

	float mass;
	float invMass;

	Vector2f position;

	std::list<Vector2f> forces;
	std::list<Vector2f> impulses;

	Vector2f acceleration;
	Vector2f velocity;
};

struct Manifold
{
	Body* body1;
	Body* body2;

	float penetration;
	Vector2f normal;
};

Body createBody(float mass, const FloatRect &aabb, const Vector2f& position)
{
	Body body { aabb, RectangleShape({ aabb.width, aabb.height }),
		mass, (mass == 0.0f) ? 0.0f : 1.0f / mass, position};

	body.debugGeometry.setFillColor(Color::Transparent);
	body.debugGeometry.setOutlineColor(Color::Red);
	body.debugGeometry.setOutlineThickness(1);

	return body;
}

class Game
{
public:
	void Run()
	{
		if (!init())
			return;

		while (m_window.isOpen())
		{
			sf::Event event;
			while (m_window.pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					m_window.close();
			}

			update();
			draw();
		}

		shutdown();
	}

private:

	bool init()
	{
		VideoMode vm(800, 600);
		m_window.create(vm, "SFML");

		// TODO: change framerate
		m_window.setFramerateLimit(120);

		m_bodies.push_back(createBody(1.0f, { 0.0f, 0.0f, 50.0f, 50.0f }, { 100.0f, 100.0f }));
		m_bodies.push_back(createBody(1.0f, { 0.0f, 0.0f, 60.0f, 60.0f }, { 300.0f, 100.0f }));

		// TODO: make objects move against each other
		//m_bodies[0].velocity = Vector2f(200.0f, -20.0f);
		//m_bodies[1].velocity = Vector2f(-200.0f, 20.0f);

		return true;
	}

	void update()
	{
		static Clock clock; //< starts the clock
		Time deltaTime = clock.restart();
		float fDeltaTimeSeconds = deltaTime.asSeconds();

		auto length2 = [](const Vector2f &vec) -> float { return vec.x * vec.x + vec.y * vec.y; };

		// interact with bodies using forces
		const float acc = 100.0f; 
		Vector2f accVec;
		if (Keyboard::isKeyPressed(Keyboard::Left))
			accVec = {-acc, 0.0f};
		if (Keyboard::isKeyPressed(Keyboard::Right))
			accVec = { acc, 0.0f };
		if (Keyboard::isKeyPressed(Keyboard::Up))
			accVec = { 0.0f, -acc };
		if (Keyboard::isKeyPressed(Keyboard::Down))
			accVec = { 0.0f, acc };

		// speed limit 
		const float speedLimit = 100.0f;
		const float squaredSpeedLimit = speedLimit * speedLimit;
		if(length2(m_bodies[0].velocity + accVec * fDeltaTimeSeconds) < squaredSpeedLimit)
			m_bodies[0].impulses.push_back(accVec);
		
		// 
		handleCollisions();

		// physical simulation loop
		for (auto& body : m_bodies)
		{
			Vector2f forces{};
			for (const auto& f : body.forces)
				forces += f;

			for (const auto& i : body.impulses)
				forces += i;
			body.impulses.clear();

			//
			body.acceleration = forces * body.invMass;
			body.velocity += body.acceleration * fDeltaTimeSeconds;
			//body.velocity = body.velocity * 0.99f; //< simple "friction"
			body.position += body.velocity * fDeltaTimeSeconds;

			err() << "force " << forces.x << " " << forces.y << " acc " << body.acceleration.x <<
				body.acceleration.y << " velocity " << body.velocity.x << " " << body.velocity.y << endl; ;
		}
	}

	bool AABBvsAABB(const FloatRect& a, const FloatRect& b, 
		Vector2f& normal, float& penetration)
	{
		auto getCenter = [](const sf::FloatRect& rect) -> Vector2f
		{
			return Vector2f(rect.left, rect.top) + 0.5f * Vector2f(rect.width, rect.height) ;
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

	void findCollisions()
	{
		for (auto itA = m_bodies.begin(); itA != m_bodies.end(); ++itA)
		{
			auto& body1 = *itA;
			for (auto itB = ++itA; itB != m_bodies.end(); ++itB)
			{
				auto& body2 = *itB;

				if (body1.mass == 0 && body2.mass == 0)
				{
					// if both object don't have a mass or body is the same skip
					continue;
				}

				Vector2f normal;
				float penetration;

				Transform body1Transform;
				body1Transform.translate(body1.position);
				Transform body2Transform;
				body2Transform.translate(body2.position);
				
				if (AABBvsAABB(body1Transform.transformRect(body1.shape), 
					body2Transform.transformRect(body2.shape), normal, penetration))
				{
					Manifold manifold;
					manifold.body1 = &body1 ;
					manifold.body2 = &body2;
					manifold.normal = normal;
					manifold.penetration = penetration;

					m_manifolds.push_back(manifold);
				}
			}
		}
	}

	void resolveCollisions()
	{
		for (auto man : m_manifolds)
		{
			// Calculate relative velocity
			Vector2f rv = man.body1->velocity - man.body2->velocity;

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
				j /= man.body1->invMass + man.body2->invMass;

				// Apply impulse
				Vector2f impulse = j * man.normal;

				// TODO: add mass and restitution
				man.body1->velocity += impulse * man.body1->invMass;
				man.body2->velocity -= impulse * man.body2->invMass;

				//err() << "velAlongNormal " << velAlongNormal << " impulse " << impulse.x << " " << impulse.y << endl;
			}
			else
			{
				// Apply impulse
				Vector2f impulse = velAlongNormal * man.normal;

				man.body1->velocity -= 0.5f * impulse;
				man.body2->velocity += 0.5f * impulse;
			}

			// Positional correction
			bool positionalCorrection = true;
			if (positionalCorrection)
			{
				const float percent = 0.2f; // usually 20% to 80%
				const float slop = 0.01f; // usually 0.01 to 0.1
				Vector2f correction = std::max(man.penetration - slop, 0.0f) /
					(man.body1->invMass + man.body2->invMass) * percent * man.normal;
				// Apply directly to position
				man.body1->position += man.body1->invMass * correction;
				man.body2->position += man.body2->invMass * correction;
			}
		}

	
	}

	void handleCollisions()
	{
		m_manifolds.clear();

		findCollisions();
		resolveCollisions();
	}

	void draw()
	{
		m_window.clear(Color::White);

		// render debug output
		for (auto& body : m_bodies)
		{
			body.debugGeometry.setPosition(body.position);
			m_window.draw(body.debugGeometry);
		}

		m_window.display();
	}

	void shutdown()
	{

	}

	RenderWindow m_window;

	vector<Body> m_bodies;
	vector<Manifold> m_manifolds;
};

int main()
{
	Game game;
	game.Run();

    return 0;
}

