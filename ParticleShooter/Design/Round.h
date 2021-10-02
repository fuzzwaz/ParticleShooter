//
//  Round.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "EnemyManager.h"
#include "Vector2.h"

#include <memory>
#include <vector>
#include <utility>

class GameObject;
class GameObjectObserver;

/*
	Each Enemy Wave is made out of multiple rounds. The start of a Round
	spawns a group of Enemies. Once all of the Enemies have been eliminated, a Round is "complete"
 */
class Round
{
public:
	void StartRound(GameObjectObserver& observer, const EnemyManager& enemyManager);
	void EndRound() { _enemies.clear(); }

	void AddEnemyToSpawn(const int& enemyType, const Vector2& position);

	bool IsRoundComplete() const;

	void Restart() { _roundStarted = false; }

private:
	std::vector<std::pair<int, Vector2>> _enemiesToSpawn; //The list of Enemies that should be spawned at the start of the round
	std::vector<std::weak_ptr<GameObject>> _enemies; //Actual pointers to spawned Enemies
	bool _roundStarted = false; 

};