#pragma once
#include "stdafx.h"
#include "Component.h"
#include <unordered_map>
#include "Event.h"

class InputComponent : public Component {
public:
	InputComponent(sf::Keyboard::Key up, sf::Keyboard::Key down, sf::Keyboard::Key left, sf::Keyboard::Key right, sf::Keyboard::Key shoot) :
		up(up), down(down), left(left), right(right), shoot(shoot) {}

	void update(GameEvent e) override;
	~InputComponent();
private:
	sf::Keyboard::Key up;
	sf::Keyboard::Key down;
	sf::Keyboard::Key left;
	sf::Keyboard::Key right;
	sf::Keyboard::Key shoot;
};
