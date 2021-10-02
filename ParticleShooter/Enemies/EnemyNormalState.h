//
//  EnemyNormalState.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "EnemyState.h"

class GraphicObject;

/*
	Defines the behavior (movement, attacks, etc..) for an enemy's "Normal" state.
	This is the initial state transitioned to after spawning. Acts as a home base that is regularly
	returned  to. 
 */
class EnemyNormalState : public EnemyState
{
public:
	EnemyNormalState() = default;
	EnemyNormalState(const EnemyNormalState& source) = default;

	std::shared_ptr<EnemyState> Clone() const override;

	virtual void OnStart(ObserverController& observerController, TransformPt& transform, GControllerPt& graphics, PController& properties) override;

	virtual EnemyStateType Update(const PlayerInfo& playerInfo, ObserverController& observerController, TransformPt& transform, GControllerPt& graphics, PController& properties) override;

	virtual EnemyStateType GetStateType() const override { return EnemyStateType::NORMAL; }

protected:
	virtual void PerformMovementBehavior(ObserverController& observerController, const PlayerInfo& playerInfo, TransformPt& transform);
	virtual void PerformAttackBehavior(const PlayerInfo& playerInfo, TransformPt& transform);

	int CalculatePendingDamage(const TransformPt& transform, ObserverController& observerController) const;
	virtual void ApplyPendingDamage(const int& pendingDamage, TransformPt& transform, PController& properties);
};