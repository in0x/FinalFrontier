#pragma once
#include "stdafx.h"
#include "GameStateManager.h"

GameStateManager::GameStateManager() {
	states = std::unordered_map<std::string, GameState*>();
	currentState = nullptr;
}

GameStateManager::~GameStateManager() {
	for (auto& go : gameObjects)
		delete go;
	delete startScreen;
	delete inputMng;
}

void GameStateManager::setState(std::string stateName) {
	if (stateName == "") return;

	GameState* state = states.at(stateName); //c++11

	if (state != currentState) {
		if ((state != nullptr) && (currentState != nullptr)) {
			currentState->exit();
		}
		
		delete currentState;
		currentState = state;
		currentState->init();
	}
}

void GameStateManager::registerState(std::string stateName, GameState* state) {
	states.emplace(stateName, state);
}

void GameStateManager::unregisterState(std::string stateName) {
	try {
		states.erase(stateName);
	}
	catch (exception) {}
}

void GameStateManager::update() {
	currentState->update();
}

void GameStateManager::draw() {
	currentState->draw();
}

GameState* GameStateManager::getCurrentState() {
	return currentState;
}

void GameStateManager::clearForNewGame() {
	for (auto& go : gameObjects)
		delete go;

	gameObjects.clear();

	inputMng->clear();
	observers.clear();
	rend.clear();
	borderManager->clear();
	physicsMng.clear();
}