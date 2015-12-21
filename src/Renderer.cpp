#include "stdafx.h"
#include "Renderer.h"


Renderer::Renderer(RenderTarget* t)
{
	target = t;
}


Renderer::~Renderer()
{
	for (auto layer : layers)
		delete layer;
}

void Renderer::clear() {
	for (auto layer : layers)
		delete layer;
	layers.clear();
}

void Renderer::addLayer(int index) {
	if (index == -1)
		layers.push_back(new LayerComponent());
	/*else if (index > layers.size()) {
		layers.push_back(new LayerComponent());
	}*/
	else {
		layers.insert(layers.begin() + index, new LayerComponent());
	}
}

void Renderer::removeLayer(int index) {
	delete layers[index];
	layers.erase(layers.begin() + index);
}

void Renderer::registerRenderComponent(RenderComponent* rc, int layerIndex)
{
	layers[layerIndex]->tiles.push_back(rc);

	View view = target->getView();
	rc->setView(view.getCenter(), view.getSize());
}

void Renderer::draw()
{

	for (auto* var : layers)
	{
		View view = target->getView();
		var->setView(view.getCenter(), view.getSize());
		target->draw(*var);
	}
}

void Renderer::unregisterRenderComponent(RenderComponent * rc, int layerIndex)
{
	std::vector<RenderComponent*>& layer = layers[layerIndex]->tiles;
	std::vector<RenderComponent*>::iterator it;

	for (it = layer.begin(); it != layer.end(); it++) {
		if (*it == rc)
			break;
	}

	if (*it != rc) {
		std::cout << "Could not find element to delete" << std::endl;
		return;
	}

	layer.erase(it);
}
