//
//  PlayerState.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "GameObjectProperties.h"
#include "PropertyController.h"

#include <memory>

enum class PlayerStateType { NORMAL = 0, DAMAGED, BOUNCE, LASER }; //All possible states that the Player class can be in. All PlayerState objects must be one of these states

class ColliderBaton;
class ObserverController;
class Player;
class Transform;
struct InputState;

//Dictates the Movement and Attack responses for the Player. The Player's current PlayerState is asked to Update on each frame.
class PlayerState
{
public:
	/* Abstract Virtual Functions */
	virtual PlayerStateType Update(std::shared_ptr<Transform> transform, PropertyController& propController, const InputState& inputState, const ObserverController& observerController) = 0;
	virtual PlayerStateType GetStateType() const = 0;

	virtual void OnStart(PropertyController& propController, const std::shared_ptr<Transform>& transform, const ObserverController& observerController) {};
	virtual void OnEnd(const std::shared_ptr<Transform>& transform) {};

protected:
	bool HasSpecialPowerAvailable(const PropertyController& propController) const
	{
		return propController.GetPropertyValue(Property::SPECIAL_POWER) > 0;
	}
};