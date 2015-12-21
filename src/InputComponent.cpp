#include "stdafx.h"
#include "InputComponent.h"

void InputComponent::update(GameEvent e) {
	if (e.pEvent != nullptr) {
		if (sf::Keyboard::isKeyPressed(up)) {
			//std::cout << "Sent event moveUp" << std::endl;
			updateObservers(GameEvent(EventType::moveUp));
		}
		if (sf::Keyboard::isKeyPressed(down)) {
			//std::cout << "Sent event moveDown" << std::endl;
			updateObservers(GameEvent(EventType::moveDown));
		}
		if (sf::Keyboard::isKeyPressed(left)) {
			//std::cout << "Sent event moveLeft" << std::endl;
			updateObservers(GameEvent(EventType::moveLeft));
		}
		if (sf::Keyboard::isKeyPressed(right)){
			//std::cout << "Sent event moveRight" << std::endl;
			updateObservers(GameEvent(EventType::moveRight));
		}
		if (sf::Keyboard::isKeyPressed(shoot)) {
			//std::cout << "Sent event shoot" << std::endl;
			updateObservers(GameEvent(EventType::shoot));
		}
	}
}

InputComponent::~InputComponent() {
	std::cout << "InputComponent" << std::endl;
}

