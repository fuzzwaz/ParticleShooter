//
//  PlayerStateBounce.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//
#pragma once

#include "PlayerStateNormal.h"
#include "Timer.h"

/*
 The Player State for the Bounce Shot Power Up. The state allows the Player to shoot Bouncing Projectiles.

 The state lasts for a specific duration and can be temporarily left if that duration has not yet completely elapsed.
 Upon return, the duration will pick up where it left off.
 */
class PlayerStateBounce : public PlayerStateNormal
{
public:
	PlayerStateBounce();

	virtual void OnStart(PropertyController& propController, const std::shared_ptr<Transform>& transform, const ObserverController& observerController) override;
	virtual PlayerStateType Update(std::shared_ptr<Transform> transform, PropertyController& propController, const InputState& inputState, const ObserverController& observerController) override;

	virtual PlayerStateType GetStateType() const { return PlayerStateType::BOUNCE; }

private:
	const float _bounceStateDurationTime = 10;
	Timer _bounceStateDurationTimer;

	bool _bouncePowerUpAlreadyInProgress = false;
};