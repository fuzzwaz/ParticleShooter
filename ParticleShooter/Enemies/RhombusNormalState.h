//
//  RhombusNormalState.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "EnemyNormalState.h"
#include "ProjectileShooter.h"

#include <memory>

/*
	Defines the behavior (movement, attacks, etc..) for a Rhombus's "Normal" state.
	This is the initial state transitioned to after spawning. Acts as a home base that is regularly
	returned  to.
 */
class RhombusNormalState : public EnemyNormalState
{
public:
	RhombusNormalState();
	RhombusNormalState(const RhombusNormalState& source);
	std::shared_ptr<EnemyState> Clone() const override;
	void OnStart(ObserverController& observerController, TransformPt& transform, GControllerPt& graphics, PController& properties) override;

protected:
	void PerformAttackBehavior(const PlayerInfo& playerInfo, TransformPt& transform) override;
	void PerformMovementBehavior(ObserverController& observerController, const PlayerInfo& playerInfo, TransformPt& transform) override;
	void ApplyPendingDamage(const int& pendingDamage, TransformPt& transform, PController& properties) override;

private:
	void RandomizeMovementTarget(TransformPt& transform);
	void ResetAttackTimer();

	/* Randomized offsets which keep the enemy movement and attacks reasonably unpredictable and varied */
	Vector2 _playerOffset; //How far off the enemy should track the player from where the player actually is
	const int _playerOffsetMagnitude = 450; //How accurate they should track the player. The further away from 0, the less accurate
	const int _speedOffsetMagnitude = 150; //Maximum amount that is applied to a Rhombus's speed to make it particularly fast or slow compared to other enemies
	const int _attackTimeOffsetMagnitude = 3000; //Maximum amount of time that can be applied to the attack timer in MS

	int _minimumSpeed = 100; //Increased randomly according to _speedOffsetMagnitude.
	const int _accelerationMagnitude = 1000;

	const int _damagedImpulseMagnitude = -150; //The magnitude for the impulse velocity applied when taking damage. Used for exaggeration and animated purposes

	const int _distanceFromPlayerToMaintain = 300; //How far away the Enemy should stay from the player

	float _attackTime = 2; //How often the Enemy should attack. Randomized by _attackTimeOffsetMagnitude
	Timer _attackTimer;

	std::unique_ptr<ProjectileShooter> _projectileShooter = nullptr;
};