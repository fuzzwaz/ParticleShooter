//
//  HexagonNormalState.h
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
	Defines the behavior (movement, attacks, etc..) for a Hexagon's "Normal" state.
	This is the initial state transitioned to after spawning. Acts as a home base that is regularly
	returned  to.

	Hexagon's continually rotate and spawn projectiles at their edges at a given time interval
 */
class HexagonNormalState : public EnemyNormalState
{
public:
	HexagonNormalState();
	HexagonNormalState(const HexagonNormalState& source);
	std::shared_ptr<EnemyState> Clone() const override;
	EnemyStateType Update(const PlayerInfo& playerInfo, ObserverController& observerController, TransformPt& transform, GControllerPt& graphics, PController& properties) override;

protected:
	void PerformAttackBehavior(const PlayerInfo& playerInfo, TransformPt& transform) override;
	void PerformMovementBehavior(ObserverController& observerController, const PlayerInfo& playerInfo, TransformPt& transform) override;

private:
	void Shoot(const TransformPt& transform, const std::shared_ptr<const std::vector<Vector2>>& spawnLocations) const;

	float _attackTime = 2.5; //How much time until the next attack. Dictates the interpolation of rotation
	Timer _attackTimer;

	double _currentAngle = 0; //Current orientation in degrees
	double _nextAngle = 0; //Next orientation that is being interpolated to. In degrees
	const double _rotationPerAttack = 29.2; //How much rotation occurs before each attack (in the span of _attackTime)

	bool _rotated = false; //Whether the current orientation is in the standard alias or the rotated alias

	std::unique_ptr<ProjectileShooter> _projectileShooter = nullptr;

	std::shared_ptr<std::vector<Vector2>> _rotatedProjectileSpawnLocations = nullptr; //Spawn locations for a rotated alias orientation
	std::shared_ptr<std::vector<Vector2>> _standardProjectileSpawnLocations = nullptr; //Spawn locations for a standard alias orientation
};