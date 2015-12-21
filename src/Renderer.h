#pragma once
#include "stdafx.h"
#include "RenderComponent.h"
#include "LayerComponent.h"

class Renderer
{
public:
	Renderer(RenderTarget *t);
	~Renderer();
	void registerRenderComponent(RenderComponent* rc, int layerIndex);
	void draw();
	void clear();
	void unregisterRenderComponent(RenderComponent* rc, int layerIndex);
	void addLayer(int index = -1);
	void removeLayer(int index);
private:
	std::vector<LayerComponent*> layers;
	RenderTarget* target;
};

