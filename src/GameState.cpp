#pragma once
#include "stdafx.h"
#include "GameState.h"
#include <algorithm>
#include "PhysicsComponent.h"
#include "HealthComponent.h"

//StartState functions:
StartState::StartState(GameStateManager* gSMng) {
	manager = gSMng;
}

StartState::~StartState() {}

void StartState::init() {
	
	manager->rend.addLayer(0);
	manager->rend.registerRenderComponent(new RenderComponent("start.png", SpriteType::BackgroundSprite), 0);

	update();
}

void StartState::update() {
	sf::Event emptyevent;
	while (manager->window.isOpen()) {
		sf::Event event;
		bool updated = false;
		while (manager->window.pollEvent(event)) {
			if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape))
				manager->window.close();
			if (sf::Keyboard::isKeyPressed(Keyboard::Space))
			{
				manager->setState("mainState");
				return;
			}
			manager->updateObservers(GameEvent(&event));
			updated = true;
		}
		if (!updated)
			manager->updateObservers(GameEvent(&emptyevent));

		draw();
		manager->window.display();
	}
}

void StartState::draw() {
	manager->rend.draw();
}

void StartState::exit() {
	manager->rend.removeLayer(0);
}


//MainState functions:
MainState::MainState(GameStateManager* gSMng) {
	manager = gSMng;
}

MainState::~MainState() {}

void MainState::init() {

	auto dim = manager->window.getSize();
	manager->window.setView(View{ FloatRect{ 0,0.f,(float)dim.x,(float)dim.y } });

	manager->unregisterState("endState");

	std::vector<GameObjectInfo> objects = manager->spriteManager.init("FinalFrontier.tmx");
	GameObject* player1 = new GameObject("player1");
	GameObject* player2 = new GameObject("player2");
	player1->addComponent(new HealthComponent(10));
	player2->addComponent(new HealthComponent(10));

	InputComponent* player1Controls = new InputComponent(sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::Space);
	player1->addComponent(player1Controls);
	manager->inputMng->addObserver(player1Controls);

	InputComponent* player2Controls = new InputComponent(sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Return);
	player2->addComponent(player2Controls);
	manager->inputMng->addObserver(player2Controls);

	std::vector<std::shared_ptr<PhysicsComponent>> physComps;

	for (auto info : objects) {

		if (info.object.name == "Player1"){
			player1->addComponent(info.components[0]);
			std::shared_ptr<PhysicsComponent> playerCollider = std::make_shared<PhysicsComponent>(info.object);
			manager->physicsMng.addComponent(playerCollider);
			player1->addComponent(playerCollider.get());
		}

		else if (info.object.name == "Player2") {
			player2->addComponent(info.components[0]);
			std::shared_ptr<PhysicsComponent> playerCollider = std::make_shared<PhysicsComponent>(info.object);
			manager->physicsMng.addComponent(playerCollider);
			player2->addComponent(playerCollider.get());
		}

		else if (info.object.type == "Collider")
			manager->physicsMng.addComponent(std::make_shared<PhysicsComponent>(info.object));
		else if (info.object.type == "Border")
		{
			shared_ptr<PhysicsComponent> pc = std::make_shared<PhysicsComponent>(info.object);
			manager->physicsMng.addComponent(pc);
			manager->borderManager->registerBorder(pc.get(),info.components);
		}
	}

	manager->addObserver(manager->inputMng);
	manager->gameObjects.push_back(player1);
	manager->gameObjects.push_back(player2);
	
	update();
}

void MainState::update() {
	static Clock clock; //< starts the clock
	
	Event *emptyevent = new sf::Event();

	while (manager->window.isOpen()) { //Please space out your code to make it a little more readable atleast

		Time deltaTime = clock.restart();

		float deltaTimeSeconds = deltaTime.asSeconds();

		manager->physicsMng.update(deltaTimeSeconds);

		sf::Event event;

		bool updated = false;

		while (manager->window.pollEvent(event)) {

			if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
				manager->window.close();
				return;
			}

			manager->updateObservers(GameEvent(&event));
			updated = true;
		}

		if (!updated)
			manager->updateObservers(GameEvent(emptyevent));

		m_fBGScrollX = lerp(0, 3647/50, deltaTimeSeconds);

		for (auto go : manager->gameObjects) {

			GameEvent ev = go->updateComponents(deltaTimeSeconds);

			if (ev.type == EventType::unitDied) {
				EndState* end = new EndState{ manager };
				manager->registerState("endState", end);
				manager->setState("endState");
				return;
			}
		}

		draw();
	}

	delete emptyevent;
}

float MainState::lerp(float a, float b, float t) {
	return a*(1 - t) + b*t;
}

void MainState::draw() {
	manager->window.clear();

	View view = manager->window.getView();
	view.move(m_fBGScrollX, 0.0f);
	manager->window.setView(view);
	manager->borderManager->setView(view);
	manager->rend.draw();
	manager->window.display();
}

void MainState::exit() {

}

EndState::EndState(GameStateManager* manager) {
	this->manager = manager;
}

void EndState::init() {
	auto dim = manager->window.getSize();
	manager->window.setView(View{ FloatRect{ 0,0.f,(float)dim.x,(float)dim.y } });

	font.loadFromFile("../assets/arial.ttf");

	background.loadFromFile("../assets/space.png");

	endScreen = sf::Sprite{ background };
	endScreen.setPosition(0, 0);

	points = sf::Text{"Your score: " + std::to_string(manager->score),font };
	points.setCharacterSize(60);
	points.setPosition(dim.x / 10, dim.y / 5);
	points.setStyle(1);

	prompt = sf::Text{ "Hit space to restart or escape to exit. \nThanks for playing!",font };
	prompt.setCharacterSize(60);
	prompt.setPosition(dim.x / 10, (dim.y / 5)*2);
	points.setStyle(1);


	this->window = &manager->window;

	update();
}

void EndState::update() {
	sf::Event e;

	while (manager->window.isOpen()) {

		while (manager->window.pollEvent(e)) {

			if (e.type == sf::Event::KeyPressed) {

				if (e.key.code == sf::Keyboard::Key::Space) {
					manager->clearForNewGame();
					MainState* newMain = new MainState{ manager };
					manager->unregisterState("mainState");
					manager->unregisterState("newMain");

					manager->registerState("newMain",newMain);
				
					manager->setState("newMain");

					return;
				}

				else if (e.key.code == sf::Keyboard::Key::Escape)
					manager->window.close();
			}
			
		}

		draw();
	}

}

void EndState::draw() {
	window->clear();
	window->draw(endScreen);
	window->draw(points);
	window->draw(prompt);
	window->display();
}

void EndState::restart() {

}