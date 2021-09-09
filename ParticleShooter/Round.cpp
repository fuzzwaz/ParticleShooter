#include "Enemy.h"
#include "GameObjectObserver.h"
#include "GameObjectRegistryLocator.h"
#include "GameObjectRegistryService.h"
#include "Round.h"

/*
	Description:
		Determines whether or not all of the spawned Enemies have been defeated. If so,
		the Round is "complete." Uses the weak pointer references to detect if the Enemies
		have been removed from memory (happens immediately after death)
*/
bool Round::IsRoundComplete() const
{
	for (const auto& enemy : _enemies)
	{
		if (!enemy.expired())
		{
			return false;
		}
	}

	return true;
}

/*
	Description:
		Goes through the list of enemies to spawn and spawns them.
		Notifies observers that the round has been started

	Arguments:
		observer - Observers that want to know that the round has started
		enemyManager - Responsible for actually creating the enemies that need to be spawned
*/

void Round::StartRound(GameObjectObserver& observer, const EnemyManager& enemyManager)
{
	_roundStarted = true;
	for (const auto& spawn : _enemiesToSpawn)
	{
		std::shared_ptr<Enemy> enemy = nullptr;
		if (spawn.first == 0)
			enemy = enemyManager.SpawnEnemy(ObjectId::ENEMY_RHOMBUS, spawn.second);
		else if (spawn.first == 1)
			enemy = enemyManager.SpawnEnemy(ObjectId::ENEMY_SQUARE, spawn.second);
		else if (spawn.first == 2)
			enemy = enemyManager.SpawnEnemy(ObjectId::ENEMY_HEXAGON, spawn.second);

		if (enemy != nullptr)
		{
			GameObjectRegistryService* registry = GameObjectRegistryLocator::GetCreatorService();
			registry->AddGameObjectToScene(enemy);
			std::weak_ptr<GameObject> enemyPtr = enemy;
			_enemies.push_back(enemyPtr);
		}
	}

	observer.Notify(GameObjectEvent::ROUND_START);
}

void Round::AddEnemyToSpawn(const int& enemyType, const Vector2& position)
{
	_enemiesToSpawn.emplace_back(std::make_pair(enemyType, position));
}