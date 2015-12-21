#pragma once
#include "stdafx.h"
#include "GameStateManager.h"
#include "InputComponent.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "Renderer.h"
#include "InputManager.h"
#include "Projectile.h"
#include "SpriteManager.h"

class GameStateManager;

class GameState abstract {
public:
	virtual void init() = 0;
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void exit() = 0;
};

class StartState : public GameState {
public:
	StartState(GameStateManager*);
	~StartState();
	void init();
	void update();
	void draw();
	void exit();
protected:
	GameStateManager* manager;
private:
	RenderComponent* background;
};

class MainState : public GameState {
public:
	MainState(GameStateManager*);
	MainState::~MainState();
	void init();
	void update();
	void draw();
	void exit();
protected:
	GameStateManager* manager;
private:
	float m_fBGScrollX;
	float lerp(float a, float b, float t);
};

class EndState : public GameState {
public:
	EndState(GameStateManager*);
	void init();
	void update();
	void draw();
	void restart();
	void exit() {};
private: 
	sf::RenderWindow* window;
	sf::Font font;
	sf::Texture background;
	sf::Sprite endScreen;
	sf::Text points;
	sf::Text prompt;

	GameStateManager* manager;
};