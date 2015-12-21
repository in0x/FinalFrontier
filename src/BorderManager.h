#pragma once
#include "PhysicsComponent.h"
#include "RenderComponent.h"

class BorderManager
{
public:
	BorderManager();
	~BorderManager();
	void registerBorder(PhysicsComponent* pc, std::vector<RenderComponent*> rc);
	void unregisterBorder(PhysicsComponent* pc, std::vector<RenderComponent*> rc);
	void clear();
	//void registerPhysicsComponent(PhysicsComponent* pc);
	//void unregisterPhysicsComponent(PhysicsComponent* pc);
	void setView(View view);
private:
	std::vector<PhysicsComponent*> p_borders;
	std::vector<std::vector<RenderComponent*>> r_borders;
	View lastView;
};

