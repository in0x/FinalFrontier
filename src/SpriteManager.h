#pragma once
#include "NLTmxMap.h"
#include "Renderer.h"
#include "SpriteComponent.h"
#include "GameObject.h"

struct GameObjectInfo {
public:
	GameObjectInfo() {};
	GameObjectInfo(RenderComponent* component, NLTmxMapObject object) : object(object) { components.push_back(component); }
	GameObjectInfo(NLTmxMapObject object) : object(object) { }

	std::vector<RenderComponent*> components;
	NLTmxMapObject object;
};

class SpriteManager {
public:
	SpriteManager(Renderer* renderer);
	std::vector<GameObjectInfo>init(std::string path);
private:
	Renderer* renderer;
	std::string assetPath = "../assets/";
	std::unordered_map<std::string, sf::Image> tilesetImages;
	std::unordered_map<std::string, shared_ptr<sf::Texture>> m_tilesetTexture;
	std::vector<std::vector<shared_ptr<sf::Sprite>>> layers;
	std::vector<shared_ptr<sf::Texture>> texPtrs;
};
