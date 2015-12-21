#pragma once

// TMX map file parser from https://github.com/catnapgames/TestNLTmxMap
// modification: getTilesetForGrid added (WL)

#include <string>
#include <vector>

using namespace std;

class NLTmxMapTileset {
public:
	int firstGid;
	string name;
	int tileWidth;
	int tileHeight;
	int spacing;
	int margin;
	string filename;
	int trans;
};

struct NLTmxMapObjectProperty {
	string name;
	string value;
};


class NLTmxMapLayer {
public:
	string name;
	int width;
	int height;
	int* data;
	vector<NLTmxMapObjectProperty*> properties;

	~NLTmxMapLayer() {
		delete[] data;
		for (auto property : properties) {
			delete property;
		}
	}
};

class NLTmxImageLayer {
public:
	string name;
	std::string path;
	vector<NLTmxMapObjectProperty*> properties;

	~NLTmxImageLayer() {
		for (auto property : properties) {
			delete property;
		}
	}
};

class NLTmxMapObject {
public:
	string name;
	string type;
	int gid;
	int x;
	int y;
	int width;
	int height;
	vector<NLTmxMapObjectProperty> properties;

public:

	~NLTmxMapObject() {
	}
};


class NLTmxMapObjectGroup {
public:
	string name;
	int width;
	int height;
	bool visible;


	vector<NLTmxMapObjectProperty*> properties;
	vector<NLTmxMapObject*> objects;

	~NLTmxMapObjectGroup() {
		for (auto property : properties) {
			delete property;
		}

		for (auto o : objects) {
			delete o;
		}
	}
};


class NLTmxMap {
public:

	int width;
	int height;
	int tileWidth;
	int tileHeight;

	vector<NLTmxMapTileset*> tilesets;
	vector<NLTmxMapLayer*> layers;
	vector<NLTmxMapObjectGroup*> groups;
	vector<NLTmxImageLayer*> imageLayers;

	~NLTmxMap() {
		for (auto g : groups) {
			delete g;
		}

		for (auto l : layers) {
			delete l;
		}

		for (auto ts : tilesets) {
			delete ts;
		}

		for (auto il : imageLayers) {
			delete il;
		}
	}

	NLTmxMapTileset* getTilesetForGrid(int grid);
};


NLTmxMap* NLLoadTmxMap(char *xml);

