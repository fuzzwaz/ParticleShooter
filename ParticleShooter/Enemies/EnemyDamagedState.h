//
//  EnemyDamagedState.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "EnemyNormalState.h"
#include "Timer.h"

/*
	Defines the behavior (movement, attacks, etc..) for an enemy's "Damaged" state.
	This is a state that is transitioned to whenever an enemy takes damage. The default
	implementation makes an enemy invisible for a period of time. 
 */
class EnemyDamagedState : public EnemyState
{
public:
	EnemyDamagedState() = default;

	std::shared_ptr<EnemyState> Clone() const override;

	void OnStart(ObserverController& observerController, TransformPt& transform, GControllerPt& graphics, PController& properties) override;

	EnemyStateType Update(const PlayerInfo& playerInfo, ObserverController& observerController, TransformPt& transform, GControllerPt& graphics, PController& properties) override;

	EnemyStateType GetStateType() const override { return EnemyStateType::DAMAGED; }

private:
	float _damagedStateTime = 0.1; //How long the Enemy is in this state
	Timer _damagedStateTimer;
};
