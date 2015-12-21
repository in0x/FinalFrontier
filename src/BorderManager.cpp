#include "stdafx.h"
#include "BorderManager.h"


BorderManager::BorderManager()
{
}


BorderManager::~BorderManager()
{
}

//void BorderManager::registerPhysicsComponent(PhysicsComponent* pc)
//{
//	borders.push_back(pc);
//}
//void BorderManager::unregisterPhysicsComponent(PhysicsComponent* pc)
//{
//	std::vector<PhysicsComponent*>::iterator it;
//
//	for (it = borders.begin(); it != borders.end(); it++) {
//		if (*it == pc)
//			break;
//	}
//
//	if (*it != pc) {
//		std::cout << "Could not find element to delete" << std::endl;
//		return;
//	}
//
//	borders.erase(it);
//}

void BorderManager::clear() {
	p_borders.clear();
	r_borders.clear();
	lastView = View{ FloatRect{ 0,0.f,lastView.getSize().x,lastView.getSize().y } };
}

void BorderManager::registerBorder(PhysicsComponent* pc, std::vector<RenderComponent*> rc)
{
	p_borders.push_back(pc);
	r_borders.push_back(rc);
}
void BorderManager::unregisterBorder(PhysicsComponent* pc, std::vector<RenderComponent*> rc)
{
	/*{
		std::vector<PhysicsComponent*>::iterator it;

		for (it = p_borders.begin(); it != p_borders.end(); it++) {
			if ((*it) == pc)
				break;
		}

		if (*it != pc) {
			std::cout << "Could not find element to delete" << std::endl;
			return;
		}

		p_borders.erase(it);
	}

	{
		std::vector<RenderComponent*>::iterator it;

		for (it = r_borders.begin(); it != r_borders.end(); it++) {
			if ((*it) == rc)
				break;
		}

		if (*it != rc) {
			std::cout << "Could not find element to delete" << std::endl;
			return;
		}

		r_borders.erase(it);
	}*/

}

void BorderManager::setView(View view)
{
	Vector2f newPos = Vector2f(view.getCenter().x - view.getSize().x/2, view.getCenter().y - view.getSize().y/ 2);
	Vector2f oldPos = Vector2f(lastView.getCenter().x - lastView.getSize().x / 2, lastView.getCenter().y - lastView.getSize().y / 2);
	Vector2f diff = newPos - oldPos;
	for (int i = 0; i < p_borders.size(); i++)
	{
		p_borders[i]->move(diff);
		for(auto render : r_borders[i])
			render->move(diff.x, diff.y);
	}
	lastView = view;
}