#pragma once

#include "PlayerState.h"
#include "Vector2.h"

class Transform;

/*
	A lean and safe info struct meant to inform other objects of the current status of the Player.
	Objects can adjust their behavior based on Player info. For instance, spawning more health
	when the Player's health is low.
 */
struct PlayerInfo
{
	Vector2 _CurrentPosition;
	PlayerStateType _CurrentPlayerState = PlayerStateType::NORMAL;
	int _CurrentHealth = 100;
	int _CurrentSpecial = 100;
};