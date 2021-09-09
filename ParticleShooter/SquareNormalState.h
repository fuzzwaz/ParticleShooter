//
//  SquareNormalState.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "EnemyNormalState.h"

#include <memory>

class GraphicObject;

/*
	Defines the behavior (movement, attacks, etc..) for a Square's "Normal" state.
	This is the initial state transitioned to after spawning. Acts as a home base that is regularly
	returned  to.

	Square movement goes through different stages:
		Stage #1: Searching for Targets
		Stage #2: Warning
		Stage #3: Attacking
		Stage #4: Time Out
		(Repeat)

	Squares can only move and see in the cardinal directions
 */
class SquareNormalState : public EnemyNormalState
{
public:
	SquareNormalState();
	SquareNormalState(const SquareNormalState& source);
	std::shared_ptr<EnemyState> Clone() const override;

protected:
	void PerformMovementBehavior(ObserverController& observerController, const PlayerInfo& playerInfo, TransformPt& transform) override;

private:
	void AbruptStopIfColliding(const TransformPt& transform);

	void FinishAttacking();
	void FinishInactiveTimeOut();
	void FinishTargetWarning();

	void SearchForTargets(const PlayerInfo& playerInfo, const TransformPt& transform, ObserverController& observerController);
	bool CheckIfTargetIsInRange(const float& targetAngle, const Vector2& closeDirection, const Vector2 farDirection);
	void BeginWarningForUpcomingAttack(const TransformPt& transform);

	/* Dictates how long the Warning Stage lasts */
	float _warningTime = 0.6;
	Timer _warningTimer;
	bool _warningInitiated = false;

	std::shared_ptr<GraphicObject> _attackWarningBeamPrototype; //Warning beam effect displayed during Warning Stage

	/* Dictates how long the Time Out Stage lasts */
	float _inactiveTime = 1;
	Timer _inactiveTimer;
	bool _inactive = false;

	/* Dictates how long the Attack Stage lasts */
	float _attackTime = 1;
	Timer _attackTimer;
	bool _attacking = false;

	const float _accelerationMagnitude = 8000; //The acceleration Square maintains during it's Attack stage

	Vector2 _attackDirection; //Once a attack is initiated, Square does not change it's direction

	/* List of possible attack directions */
	const Vector2 UP = Vector2(0, -1);
	const Vector2 DOWN = Vector2(0, 1);
	const Vector2 RIGHT = Vector2(1, 0);
	const Vector2 LEFT = Vector2(-1, 0);

	const int _targetSeekingHalfAngle = 5; //If the target (Player) is within the Seeking angle of any of the attack directions, they are "found" by the Square enemy
};