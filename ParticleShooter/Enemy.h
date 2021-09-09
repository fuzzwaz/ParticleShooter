//
//  Enemy.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "EnemyManager.h"
#include "EnemyNormalState.h"
#include "EnemyState.h"
#include "GraphicObject.h"

#include <unordered_map>

class Animation;
class Texture;
class Sound;
struct Vector2;

class Enemy : public GraphicObject
{
public:
	Enemy();
	Enemy(const Enemy& source);

	std::shared_ptr<Enemy> Clone(const Vector2& position) const;

	void SetEnemyState(const EnemyStateType& type, const std::shared_ptr<EnemyState>& state);

	virtual void Update(const PlayerInfo& playerInfo, const Vector2& cameraPosition, const InputState& input) override;
	ColliderInterface* GetCollider() override;

private:
	friend class EnemyManager;

	void StartCurrentState();
	void SwitchEnemyState(const EnemyStateType& type);
	void Destroyed();

	std::shared_ptr<EnemyState> _state = nullptr; //The current state that the enemy is in
	std::unordered_map<EnemyStateType, std::shared_ptr<EnemyState>> _stateMappings; //A mapping of each state type and the specific EnemyState that goes along with it (differs per Enemy)

	std::shared_ptr<GraphicObject> _destructionParticlesPrototype = nullptr; //Particles that show up where the enemy gets destroyed
};
