#include "stdafx.h"
#include "LayerComponent.h"

LayerComponent::LayerComponent() {}

LayerComponent::~LayerComponent() {
	for (auto& component : tiles) {
		if (component == nullptr)
			continue;
		delete component;
		component = nullptr;
	}
}

void LayerComponent::setView(Vector2f vPos, Vector2f vSize) {
	for (auto comp : tiles)
		comp->setView(vPos, vSize);
}

void LayerComponent::draw(RenderTarget & target, RenderStates states) const {
	for (auto comp : tiles) {
		comp->draw(target, states);
	}
}