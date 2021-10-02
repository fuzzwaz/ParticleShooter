//
//  PlayerStateLaser.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "PlayerStateNormal.h"
#include "Timer.h"

class Laser;

//The Player State for the Laser Special Ability Attack. Requires the Player to have Special Ability power to enter and stay in this state
class PlayerStateLaser : public PlayerStateNormal
{
public:
	PlayerStateLaser();

	virtual void OnStart(PropertyController& propController, const std::shared_ptr<Transform>& transform, const ObserverController& observerController) override;
	virtual PlayerStateType Update(std::shared_ptr<Transform> transform, PropertyController& propController, const InputState& inputState, const ObserverController& observerController) override;

	virtual PlayerStateType GetStateType() const { return PlayerStateType::LASER; }

private:
	void DepleteSpecialAbilityBar(PropertyController& propController);

	/* Time in which the Player's special ability power depletes */
	const float _specialAbilityDepleteTime = 0.125;
	Timer _specialAbilityDepleteTimer;

	const float _movementSpeedSpecial = 350; //Player is slowed down to a different movement speed while Special is active
	const int _specialAbilityDepleteAmount = 5;
	std::shared_ptr<Laser> _laser = nullptr;
};