// FinalFrontier.cpp : Defines the entry point for the console application.
//

#pragma once
#include "stdafx.h"
#include "Event.h"
#include "InputManager.h"
#include "InputComponent.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "RenderComponent.h"
#include "Renderer.h"
#include "Projectile.h"
#include "GameStateManager.h"

int main()
{
	GameStateManager* game = new GameStateManager();
	GameState* startState = new StartState(game); 
	GameState* mainState = new MainState(game); 
	//EndState* endState = new EndState{ game };  

	game->registerState("startState", startState);
	game->registerState("mainState", mainState);
	//game->registerState("endState", endState);
	game->setState("startState");
	
	delete mainState;
	delete game;

    return 0;
}
