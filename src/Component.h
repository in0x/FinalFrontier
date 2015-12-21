#pragma once
#include "stdafx.h"
#include "Event.h"

class Component : public Observer, public Observable {
public:
	virtual ~Component() {}
};
