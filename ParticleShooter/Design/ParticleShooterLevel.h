//
//  ParticleShooterLevel.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "Level.h"

/*
	Represents a single level in the game. Levels are made out of Waves and are complete
	when each Wave is complete. Waves are comprised of Rounds and are complete when each Round
	is complete. Rounds are comprised of Enemies and are complete when each Enemy has been defeated. 
*/
class ParticleShooterLevel : public Level
{
public:
	virtual void Start() override;
	virtual void Restart() override;

	virtual void Update(GameObjectObserver& observer) override;

	Wave* GetWave(const int& waveNumber);
	bool LevelHasFinished() const override { return _levelComplete; }

	virtual void LoadLevel() override = 0;

	/* Getters */
	virtual Vector2 GetCameraStart() const = 0;
	virtual Vector2 GetPlayerStart() const = 0;
	virtual std::shared_ptr<const Transform> GetTransform() const = 0;
	virtual std::shared_ptr<const GraphicsController> GetGraphicsController() const = 0;
	virtual std::vector<ColliderInterface*> GetLevelColliders() override = 0;

private:
	void UpdateCurrentWave(GameObjectObserver& observer, const EnemyManager& enemyManager);
	void TransitionToNextWave(GameObjectObserver& observer);
	void StartCurrentWave(GameObjectObserver& observer);

	std::unordered_map<int, Wave> _waves; //List of waves in the level
	int _currentWave = 0; //Key to the currently active wave
	bool _levelComplete = false;

	EnemyManager _enemyManager; //Houses spawning information for all ParticleShooter enemies

	const float _startDelayTime = 3;
	Timer _startDelayTimer;
	bool _delayLevel = true;
};