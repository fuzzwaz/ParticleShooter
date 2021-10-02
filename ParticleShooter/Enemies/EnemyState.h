//
//  EnemyState.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "AnimatedSingleTextureGraphicsController.h"
#include "PropertyController.h"
#include "ObserverController.h"

#include <memory>


class Enemy;
class Transform;
struct PlayerInfo;
struct Vector2;
struct InputState;

typedef std::shared_ptr<Transform> TransformPt;
typedef std::shared_ptr<AnimatedSingleTextureGraphicsController> GControllerPt;
typedef PropertyController PController;

enum EnemyStateType { SPAWNING = 0, NORMAL, DAMAGED, DESTROYED }; //All possible Enemy State categories

/*
	Defines the behavior (movement, attacks, etc..) for an enemy in the form of "EnemyState"s.
	An Enemy object calls "Update" for whichever EnemyState is currently active on each frame.
 */
class EnemyState
{
public:
	//Called immediately once the State is activated 
	virtual void OnStart(ObserverController& observerController, TransformPt& transform, GControllerPt& graphics, PController& properties) {}

	//Called right before a transitioning to a new State
	virtual void OnEnd() {}
	 
	virtual std::shared_ptr<EnemyState> Clone() const = 0;

	//Called every frame this State is active. Returns a suggested EnemyStateType for the Enemy to transition to for the next frame
	virtual EnemyStateType Update(const PlayerInfo& playerInfo, ObserverController& observerController, TransformPt& transform, GControllerPt& graphics, PController& properties) = 0;

	virtual EnemyStateType GetStateType() const = 0;

	virtual ~EnemyState() {}
};