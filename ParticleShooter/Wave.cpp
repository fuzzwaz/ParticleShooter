#include "Wave.h"

/*
	Description:
		Gets a pointer to the Round object associated with the given number.
		If the object does not exist, creates a new one and adds it to _rounds

	Argument:
		roundNumber - The integer number associated with a round

	Return:
		Round* - A pointer to a Round stored in _rounds
*/
Round* Wave::GetRound(const int& roundNumber)
{
	auto roundIt = _rounds.find(roundNumber);
	if (roundIt == _rounds.end())
		_rounds.emplace(roundNumber, Round());

	return &_rounds[roundNumber];
}

void Wave::StartWave()
{
	_waveStarted = true;
	_startDelay = true;
	_waveStartTimer.Start();
}

/*
	Description:
		Keeps track of the wave delay timer. Once that elapses, the wave officially begins.
		After the wave begins, Update keeps track of the progress of each of its Rounds and starts
		the next Round when the previous one has completed.
*/
void Wave::Update(GameObjectObserver& observer, const EnemyManager& enemyManager)
{
	if (_startDelay)
	{
		if (_waveStartTimer.GetSeconds() >= _waveStartTime)
		{
			_rounds[_currentRound].StartRound(observer, enemyManager);
			_waveStartTimer.Stop();
			_startDelay = false;
		}
	}
	else
	{
		if (_rounds[_currentRound].IsRoundComplete())
		{
			_rounds[_currentRound].EndRound();
			_currentRound++;
			if (_rounds.find(_currentRound) != _rounds.end())
			{
				_rounds[_currentRound].StartRound(observer, enemyManager);
			}
		}
	}
}

void Wave::Restart()
{
	_currentRound = 0;
	_waveStarted = false;
	_startDelay = true;
	for (auto& round : _rounds)
	{
		round.second.Restart();
	}
}