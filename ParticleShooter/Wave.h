//
//  Wave.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "Enemy.h"
#include "Round.h"
#include "Timer.h"

#include <unordered_map>

/*
	Each Level is made out of multiple "Waves". Waves represent an elevation
	in enemy difficulty. Each Wave has an array of Rounds. Once each Round is complete,
	the Wave is complete.
 */
class Wave
{
public:
	void Restart();

	Round* GetRound(const int& roundNumber);

	void StartWave();

	void Update(GameObjectObserver& observer, const EnemyManager& enemyManager);

	bool WaveStarted() const { return _waveStarted; }
	bool WaveComplete() const { return _rounds.size() == _currentRound; }

private:
	std::unordered_map<int, Round> _rounds; //All of the enemy rounds associated with the wave
	int _currentRound = 0; //The key for the round that is currently being played 
	bool _waveStarted = false; 

	bool _startDelay = true;
	const float _waveStartTime = 3; //delay time before the Wave official starts its first Round
	Timer _waveStartTimer;
};