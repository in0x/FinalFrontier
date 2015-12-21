#pragma once
#include "stdafx.h"
#include "GameState.h"
#include "Event.h"
#include "Renderer.h"
#include "GameObject.h"
#include "InputManager.h"
#include "SpriteManager.h"
#include "PhysicsManager.h"
#include "BorderManager.h"

class GameState;
class InputManager;
class GameObject;

class GameStateManager : public Observable {
private:
	GameState *currentState;
	std::unordered_map<std::string, GameState*> states;

public:
	GameStateManager();
	~GameStateManager();
	void update();
	void draw();
	void setState(std::string stateName);
	void registerState(std::string stateName, GameState* state);
	void unregisterState(std::string stateName);
	void clearForNewGame();
	GameState* getCurrentState();

	sf::RenderWindow& window = RenderWindow(VideoMode(1920, 960), "FINAL FRONTIERZ");
	std::vector<GameObject*> gameObjects = std::vector<GameObject*>();
	PhysicsManager physicsMng{};
	InputManager* inputMng = new InputManager();
	GameObject* startScreen = new GameObject("startScreen");
	RenderComponent* spaceBackground = nullptr;
	Renderer rend = Renderer(&window);
	SpriteManager spriteManager = SpriteManager(&rend);
	BorderManager* borderManager = new BorderManager();
	double score = 0;
};