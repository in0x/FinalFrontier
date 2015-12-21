#pragma once
#include "stdafx.h"
#include "Event.h"
#include "InputEventEnum.h"

class InputManager : public Observable, public Observer {
public:
	void update(GameEvent e) override;
	void clear();
};