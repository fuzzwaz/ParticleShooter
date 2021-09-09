#include "ParticleShooterLevel.h"

void ParticleShooterLevel::Start()
{
	Level::Start();
	_startDelayTimer.Start();
}


/*
	Description:
		Restarts the Level without throwing out persistent data (collider or graphic information, for example).
		Asks each Wave to Restart itself so we don't have to reconstruct all of the Level ways.
 */
void ParticleShooterLevel::Restart()
{
	Level::Restart();

	_currentWave = 0;
	_levelComplete = false;
	_delayLevel = true;
	_startDelayTimer.Start();
	for (auto& wave : _waves)
	{
		wave.second.Restart();
	}
}

/*
	Description:
		If the level is still ongoing, Update the current wave
 */

void ParticleShooterLevel::Update(GameObjectObserver& observer)
{
	if (_levelComplete)
		return;

	Level::Update(observer);

	//Delay the level start by a few seconds (_startDelayTime) to give the Player a breath
	if (_delayLevel)
	{
		if (_startDelayTimer.GetSeconds() >= _startDelayTime)
		{
			_delayLevel = false;
			_startDelayTimer.Stop();
		}
	}
	else
	{
		if (LevelHasStarted())
		{
			UpdateCurrentWave(observer, _enemyManager);
		}
	}
}

/*
	Description:
		Handles Wave management. Transitions to the next wave if the current one is complete.
		Starts Waves if they have not been started.
 */
void ParticleShooterLevel::UpdateCurrentWave(GameObjectObserver& observer, const EnemyManager& enemyManager)
{
	Wave& wave = _waves[_currentWave];

	if (wave.WaveComplete()) //Wave is complete, time to transition to the next
	{
		TransitionToNextWave(observer);
	}

	if (!wave.WaveStarted()) //Wave has not yet been started, so start the wave
	{
		StartCurrentWave(observer);
	}

	wave.Update(observer, enemyManager);
}

void ParticleShooterLevel::StartCurrentWave(GameObjectObserver& observer)
{
	Wave& wave = _waves[_currentWave];
	wave.StartWave();
	if (_waves.size() - 1 == _currentWave)
		observer.Notify(GameObjectEvent::FINAL_WAVE_START);
	else
		observer.Notify(GameObjectEvent::WAVE_START);
}

void ParticleShooterLevel::TransitionToNextWave(GameObjectObserver& observer)
{
	_currentWave++;
	if (_waves.find(_currentWave) == _waves.end())
	{
		_levelComplete = true;
		observer.Notify(GameObjectEvent::VICTORY);
	}
}

Wave* ParticleShooterLevel::GetWave(const int& waveNumber)
{
	auto roundIt = _waves.find(waveNumber);
	if (roundIt == _waves.end())
		_waves.emplace(waveNumber, Wave());

	return &_waves[waveNumber];

}

