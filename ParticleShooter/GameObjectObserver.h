//
//  GameObjectObserver.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include <list>
#include <memory>

class GameObject;

/* List of Events that a GameObjectObserver can respond to */
enum class GameObjectEvent
{
	DESTROYED,
	ENEMY_DEATH,
	ENEMY_BASIC_DAMAGE,
	ENEMY_SPECIAL_DAMAGE,
	ENEMY_BASIC_ATTACK,
	ENEMY_SPECIAL_ATTACK,
	ENEMY_TARGET_LOCKED,
	SPECIAL_ATTACK,
	BASIC_ATTACK,
	BOUNCE_ATTACK,
	POWERED_UP,
	POWERED_DOWN,
	PLAYER_DAMAGE,
	PLAYER_HEALTH_UP,
	LEVEL_START,
	ROUND_START,
	WAVE_START,
	FINAL_WAVE_START,
	GAME_OVER,
	VICTORY
};

/*
	Classes that want to observe GameObjects just need to inherit GameObjectObserver and define the Notify functions that
	correspond with the events they are interested in responding to.
*/
class GameObjectObserver
{
public:
	virtual void Notify(const GameObjectEvent& eventType, const bool begin)
	{
		if (begin)
			Notify(eventType);
	}

	virtual void Notify(const GameObjectEvent& eventType, const std::list<std::shared_ptr<GameObject>>::iterator& objectId)
	{
		Notify(eventType);
	}

	virtual void Notify(const GameObjectEvent& eventType)
	{
		 
	}
};
