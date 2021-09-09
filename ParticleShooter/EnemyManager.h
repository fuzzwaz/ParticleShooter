//
//  EnemyManager.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "Common.h"

#include <memory>
#include <unordered_map>

class Enemy;

class EnemyManager
{
public:
	EnemyManager();

	std::shared_ptr<Enemy> SpawnEnemy(const ObjectId& enemyId, const Vector2& position) const;

private:
	void ConstructRhombusPrototype();
	void ConstructSquarePrototype();
	void ConstructHexagonPrototype();

	std::unordered_map<ObjectId, const Enemy> _enemyMappings;
};