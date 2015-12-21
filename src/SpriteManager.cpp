#pragma once
#include "stdafx.h"
#include "SpriteManager.h"
#include <unordered_map>

SpriteManager::SpriteManager(Renderer* renderer) {
	this->renderer = renderer;
}

std::vector<GameObjectInfo> SpriteManager::init(std::string path) {
	//creates all render objects and gives them to renderer / gameobjects

	std::vector<GameObjectInfo> gameObjectRenderComponents;
	std::unordered_map<std::string, std::vector<RenderComponent*>> physicsRenderComps{};

	FileInputStream mapStream;
	if (!mapStream.open(assetPath + path))
	{
		throw std::invalid_argument("Mapload: Could not open file" + path);
	}

	char* mapBuffer = new char[mapStream.getSize() + 1];
	mapStream.read(mapBuffer, mapStream.getSize());
	mapBuffer[mapStream.getSize()] = '\0';

	NLTmxMap* tilemap = NLLoadTmxMap(mapBuffer);
	delete mapBuffer;

	//allocate space for all layers in renderer
	int layerCount = tilemap->imageLayers.size() + tilemap->layers.size() + tilemap->groups.size();
	for (int i = 0; i < layerCount; i++)
		renderer->addLayer();

	for (auto tileset : tilemap->tilesets)
	{
		shared_ptr<Texture> texture = make_shared<Texture>();
		if (tileset->trans != -1) {
			sf::Image tempImg;
			if (!tempImg.loadFromFile(assetPath + tileset->filename))
				std::cout << "Could not load texture " << assetPath + tileset->filename << endl;

			tempImg.createMaskFromColor(sf::Color(tileset->trans * 256 + 255));
			tilesetImages[tileset->name] = tempImg;
			texture->loadFromImage(tempImg);
		}
		else {
			if (!texture->loadFromFile(assetPath + tileset->filename))
				std::cout << "Could not load texture " << assetPath + tileset->filename << endl;
		}
		m_tilesetTexture[tileset->name] = texture;
	}

	for (int layerIdx = 0; layerIdx < (int)tilemap->layers.size(); layerIdx++)
	{
		layers.push_back(std::vector<shared_ptr<sf::Sprite>>());
		
		NLTmxMapLayer* layer = tilemap->layers[layerIdx];
		int size = layer->width * layer->height;

		int order = 0;

		for (auto property : tilemap->layers[layerIdx]->properties) { 
			std::string trimmed = property->name;
			trimmed.erase(trimmed.find_last_not_of(" \n\r\t") + 1);
			if (trimmed == "Order") {
				order = stoi(property->value, NULL, 10);
			}
		}

		bool isColliderLayer = false;
		std::string key = "";
		for (auto property : layer->properties) {
			if (property->value == "Collider" || property->value == "Border") {
				key = layer->name;
				physicsRenderComps.emplace(key, std::vector<RenderComponent*>{});
				isColliderLayer = true;
			}
		}
		
		// go over all elements in the layer
		for (int i = 0; i < size; i++)
		{
			int grid = layer->data[i];
			if (grid == 0)
			{
				// 0 means there is no tile at this grid position.

				// This continue also indicates that the layers are not 
				// represented as two-dimensional arrays, but 
				// only a list of tiles sorted by position from top left
				// to bottom right. (Hint: position could be useful for 
				// quickly finding, which tiles to cull from current 
				// viewport.)					
				continue;
			}
			
			NLTmxMapTileset* tileset = tilemap->getTilesetForGrid(grid);
			Vector2i tileSize(tileset->tileWidth, tileset->tileHeight);
			shared_ptr<sf::Texture> texture = m_tilesetTexture[tileset->name];

			int tileCountX = texture->getSize().x / tileSize.x;

			Vector2f position;
			position.x = (i % layer->width) * (float)tileSize.x;
			position.y = (i / layer->width) * (float)tileSize.y;
			
			int idx = grid - tileset->firstGid;
			int idxX = idx % tileCountX;
			int idxY = idx / tileCountX;

			int margin = tileset->margin;
			int spacing = tileset->spacing;

			IntRect source(margin + idxX * spacing + idxX * tileSize.x, margin+ idxY * spacing +idxY * tileSize.y, tileSize.x, tileSize.y);

			shared_ptr<sf::Sprite> temp = make_shared<sf::Sprite>();
			temp->setTexture(*texture);
			temp->setTextureRect(source);
			temp->setPosition(position);
			//layers[layerIdx].push_back(temp);

			SpriteComponent* sc = new SpriteComponent(temp.get(), SpriteType::TileSprite);
			RenderComponent* rc = new RenderComponent(sc);
			
			renderer->registerRenderComponent(rc, order);
			rc->setPosition(position.x, position.y);
			
			if (isColliderLayer)
				physicsRenderComps.at(layer->name).push_back(rc);
		}
		
	}

	for (int layerIdx = 0; layerIdx < tilemap->imageLayers.size(); layerIdx++) {
		std::string path = tilemap->imageLayers[layerIdx]->path;
		int idx = 0;
		for (auto property : tilemap->imageLayers[layerIdx]->properties) {
			if (property->name == "Order")
				idx = stoi(property->value);
		}

		RenderComponent* rc = new RenderComponent(assetPath + path, SpriteType::BackgroundSprite);
		renderer->registerRenderComponent(rc, idx);
	}
	
	for (auto group : tilemap->groups)
	{
		int layerIdx = 0;
		for (auto property : group->properties) {
			if (property->name == "Order")
				layerIdx = stoi(property->value);
		}

		for (auto& object : group->objects)
		{
			int grid = object->gid;

			Vector2f position;
			position.x = object->x;
			position.y = object->y;
			
			GameObjectInfo go {};
			
			//Generate Info for GameObjects that use tiles directly, such as Playes
			if (tilemap->getTilesetForGrid(grid) != nullptr) {
				NLTmxMapTileset* tileset = tilemap->getTilesetForGrid(grid);
				Vector2i tileSize(tileset->tileWidth, tileset->tileHeight);
				shared_ptr<sf::Texture> texture = m_tilesetTexture[tileset->name];

				int tileCountX = texture->getSize().x / tileSize.x;

				int idx = grid - tileset->firstGid;
				int idxX = idx % tileCountX;
				int idxY = idx / tileCountX;

				int margin = tileset->margin;
				int spacing = tileset->spacing;

				IntRect source(margin + idxX * spacing + idxX * tileSize.x, margin + idxY * spacing + idxY * tileSize.y, tileSize.x, tileSize.y);

				shared_ptr<sf::Sprite> temp = make_shared<sf::Sprite>();
				temp->setTexture(*texture);
				temp->setTextureRect(source);
				temp->setPosition(position);
				temp->setScale(3.f, 3.f);

				SpriteComponent* sc = new SpriteComponent(temp.get(), SpriteType::PlayerSprite);
				RenderComponent* rc = new RenderComponent(sc);
				renderer->registerRenderComponent(rc, layerIdx);
				rc->setPosition(position.x, position.y);
				go = GameObjectInfo{ rc, *object };
			}
			else if (object->type == "Collider" || object->type == "Border") {
				go = GameObjectInfo{ *object};
				if(physicsRenderComps.count(object->name)>0)
				for (auto rc : physicsRenderComps.at(object->name))
					go.components.push_back(rc);
			} else
				go = GameObjectInfo{ *object };

			gameObjectRenderComponents.push_back(go);
		}
	}

	delete tilemap;
	return gameObjectRenderComponents;
}