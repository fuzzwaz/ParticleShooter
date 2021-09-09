//
//  EnemySpawningState.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "EnemyState.h"

/*
	Defines the behavior (movement, attacks, etc..) for an enemy's "Spawn" state.
	This is the very first state that an Enemy has. It's responsible for playing the spawn
	animations and then transitioning to the "Normal" state
 */
class EnemySpawningState : public EnemyState
{
public:
	std::shared_ptr<EnemyState> Clone() const override;

	void OnStart(ObserverController& observerController, TransformPt& transform, GControllerPt& graphics, PController& properties) override;

	EnemyStateType Update(const PlayerInfo& playerInfo, ObserverController& observerController, TransformPt& transform, GControllerPt& graphics, PController& properties) override;

	EnemyStateType GetStateType() const override { return EnemyStateType::SPAWNING; }

private:

	const float _warningTime = 2.25;//How long the Warning animation should be played
	bool _warningComplete = false; //Whether or not the Warning stage is over (once _warningTime has been elapsed)

	const float _spawnTime = 2.5; //Total time spent in the Spawn state
	Timer _spawnTimer;
};