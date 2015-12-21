#pragma once
#include "stdafx.h"
#include "InputManager.h"

void InputManager::update(GameEvent e) {
	updateObservers(e);
}

void InputManager::clear() {
	observers.clear();
}
