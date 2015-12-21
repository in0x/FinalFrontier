#include "StdAfx.h"
#include <cassert>
#include <iostream>

#include "rapidxml/rapidxml.hpp"
#include "NLTmxMap.h"


using namespace rapidxml;

NLTmxMapTileset* NLTmxMap::getTilesetForGrid(int grid)
{
	NLTmxMapTileset* result = nullptr;

	for (auto tileset : tilesets)
	{
		if (tileset->firstGid > grid)
		{
			break;
		}

		result = tileset;
	}

	return result;
}

NLTmxMap* NLLoadTmxMap(char *xml)
{
	xml_document<> doc;
	doc.parse<0>(xml);

	xml_node<> *mapnode = doc.first_node("map");

	NLTmxMap* map = new NLTmxMap();

	map->width = atoi(mapnode->first_attribute("width")->value());
	map->height = atoi(mapnode->first_attribute("height")->value());
	map->tileWidth = atoi(mapnode->first_attribute("tilewidth")->value());
	map->tileHeight = atoi(mapnode->first_attribute("tileheight")->value());

	xml_node<> *tilesetnode = mapnode->first_node("tileset");

	while (tilesetnode) {
		NLTmxMapTileset* tileset = new NLTmxMapTileset();

		tileset->firstGid = atoi(tilesetnode->first_attribute("firstgid")->value());
		tileset->name = tilesetnode->first_attribute("name")->value();
		tileset->tileWidth = atoi(tilesetnode->first_attribute("tilewidth")->value());
		tileset->tileHeight = atoi(tilesetnode->first_attribute("tileheight")->value());
		tileset->filename = tilesetnode->first_node("image")->first_attribute("source")->value();
		
		if (tilesetnode->first_node("image")->first_attribute("trans") == NULL)
			tileset->trans = -1;
		else
			tileset->trans = strtol(tilesetnode->first_node("image")->first_attribute("trans")->value(), NULL, 16);
		
		if (tilesetnode->first_attribute("spacing") == NULL)
			tileset->spacing = 0;
		else
			tileset->spacing = atoi(tilesetnode->first_attribute("spacing")->value());

		if (tilesetnode->first_attribute("margin") == NULL)
			tileset->margin = 0;
		else
			tileset->margin = atoi(tilesetnode->first_attribute("margin")->value());

		//cout << "Tileset " << tileset->name << " filename " << tileset->filename << endl;

		map->tilesets.push_back(tileset);

		tilesetnode = tilesetnode->next_sibling("tileset");
	}

	const char *separators = " \t,\n\r";

	xml_node<> *layernode = mapnode->first_node("layer");

	while (layernode) {
		NLTmxMapLayer* layer = new NLTmxMapLayer();

		layer->name = layernode->first_attribute("name")->value();
		layer->width = atoi(layernode->first_attribute("width")->value());
		layer->height = atoi(layernode->first_attribute("height")->value());

		const char* data = layernode->first_node("data")->value();

		assert(strcmp(data, "csv"));

		layer->data = new int[layer->width * layer->height];

		char* copy = (char*)malloc(strlen(data) + 1);
		strcpy(copy, data);
		char* item = strtok(copy, separators);

		int index = 0;
		while (item) {
			layer->data[index] = atoi(item);
			index++;

			item = strtok(0, separators);
		}

		xml_node<> *propertiesnode = layernode->first_node("properties");

		if (propertiesnode) {

			xml_node<> *propertynode = propertiesnode->first_node("property");

			while (propertynode) {
				NLTmxMapObjectProperty* property = new NLTmxMapObjectProperty();
				property->name = propertynode->first_attribute("name")->value();
				property->value = propertynode->first_attribute("value")->value();

				layer->properties.push_back(property);

				propertynode = propertynode->next_sibling("property");
			}
		}

		free(copy);

		map->layers.push_back(layer);

		layernode = layernode->next_sibling("layer");
	}

	xml_node<> *imageLayerNode = mapnode->first_node("imagelayer");

	while (imageLayerNode) {
		NLTmxImageLayer* layer = new NLTmxImageLayer();

		layer->name = imageLayerNode->first_attribute("name")->value();
		layer->path = imageLayerNode->first_node("image")->first_attribute("source")->value();

		xml_node<> *propertiesnode = imageLayerNode->first_node("properties");

		if (propertiesnode) {

			xml_node<> *propertynode = propertiesnode->first_node("property");

			while (propertynode) {
				NLTmxMapObjectProperty* property = new NLTmxMapObjectProperty();
				property->name = propertynode->first_attribute("name")->value();
				property->value = propertynode->first_attribute("value")->value();

				layer->properties.push_back(property);

				propertynode = propertynode->next_sibling("property");
			}
		}

		map->imageLayers.push_back(layer);

		imageLayerNode = imageLayerNode->next_sibling("imagelayer");
	}

	xml_node<> *objectgroupnode = mapnode->first_node("objectgroup");

	while (objectgroupnode) {
		NLTmxMapObjectGroup* group = new NLTmxMapObjectGroup();

		group->name = objectgroupnode->first_attribute("name")->value();
		//group->width = atoi( objectgroupnode->first_attribute( "width" )->value() );
		//group->height = atoi( objectgroupnode->first_attribute( "height" )->value() );

		xml_node<> *propertiesnode = objectgroupnode->first_node("properties");

		if (propertiesnode) {

			xml_node<> *propertynode = propertiesnode->first_node("property");

			while (propertynode) {
				NLTmxMapObjectProperty* property = new NLTmxMapObjectProperty();
				property->name = propertynode->first_attribute("name")->value();
				property->value = propertynode->first_attribute("value")->value();

				group->properties.push_back(property);

				propertynode = propertynode->next_sibling("property");
			}
		}


		xml_attribute<> *visibleattr = objectgroupnode->first_attribute("visible");
		if (visibleattr) {
			group->visible = (atoi(visibleattr->value()) != 0);
		}
		else {
			group->visible = true;
		}

		//cout << "group " << group->name << endl;

		xml_node<> *objectnode = objectgroupnode->first_node("object");

		while (objectnode) {
			NLTmxMapObject* object = new NLTmxMapObject();

			auto nameattr = objectnode->first_attribute("name");
			if (nameattr) {
				object->name = nameattr->value();
			}
			auto gidattr = objectnode->first_attribute("gid");
			if (gidattr) {
				object->gid = atoi(gidattr->value());
			}
			auto typeattr = objectnode->first_attribute("type");
			if (typeattr) {
				object->type = typeattr->value();
			}
			object->x = atoi(objectnode->first_attribute("x")->value());
			object->y = atoi(objectnode->first_attribute("y")->value());

			auto widthattr = objectnode->first_attribute("width");
			if (widthattr) {
				object->width = atoi(widthattr->value());
			}

			auto heightattr = objectnode->first_attribute("height");
			if (heightattr) {
				object->height = atoi(heightattr->value());
			}

			xml_node<> *propertiesnode = objectnode->first_node("properties");

			if (propertiesnode) {

				xml_node<> *propertynode = propertiesnode->first_node("property");

				while (propertynode) {
					NLTmxMapObjectProperty property = NLTmxMapObjectProperty();
					property.name = propertynode->first_attribute("name")->value();
					property.value = propertynode->first_attribute("value")->value();

					object->properties.push_back(property);

					propertynode = propertynode->next_sibling("property");
				}
			}

			group->objects.push_back(object);

			objectnode = objectnode->next_sibling("object");
		}

		map->groups.push_back(group);

		objectgroupnode = objectgroupnode->next_sibling("objectgroup");
	}

	//free( (void*) xml );

	return map;
}

