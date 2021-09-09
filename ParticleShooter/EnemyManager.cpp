#include "ColliderResources.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "ErrorHandler.h"
#include "GraphicAssetResources.h"
#include "HexagonNormalState.h"
#include "RhombusNormalState.h"
#include "SquareNormalState.h"
#include "Transform.h"
#include "TriangleExplosion.h"

/*
	Description:
		Loads in the textures, animations, and transform information for each of the enemies.
		Saves prototypes that can be easily cloned at any time
*/
EnemyManager::EnemyManager()
{
	ConstructRhombusPrototype();
	ConstructSquarePrototype();
	ConstructHexagonPrototype();
}

/*
	Description:
		Checks to see if a prototype has been created for the given Id. If so, creates a clone
		of that enemy while sharing as much resources as possible (textures, animation tree, etc..).
		Returns nullptr if the enemy prototype can't be found.

	Arguments:
		enemyId - The Id for the enemy type to be cloned
		position - Where the clone will be spawned
*/
std::shared_ptr<Enemy> EnemyManager::SpawnEnemy(const ObjectId& enemyId, const Vector2& position) const
{
	const auto found = _enemyMappings.find(enemyId);
	if (found != _enemyMappings.end())
	{
		return (*found).second.Clone(position);
	}

	ErrorHandler::Assert(false, "Tried to spawn an enemy that has not been constructed");
	return nullptr;
}

void EnemyManager::ConstructRhombusPrototype()
{
	const float elasticity = 0.85, mass = 10,speed = 300, minCollisionImpulse = 500;
	const int health = 25, width = 64, height = 96;

	Enemy rhombus;
	/* Setup Transform Properties */
	rhombus._transform->SetSize(width, height);
	rhombus._transform->_RigidBody.SetElasticityCoefficient(elasticity);
	rhombus._transform->_RigidBody.SetInvertedMass(1 / mass);
	rhombus._transform->_RigidBody.SetSpeed(speed);
	rhombus._transform->_RigidBody.SetMinCollisionImpulseMagnitude(minCollisionImpulse);
	rhombus._transform->_Collider = Resources::Colliders::ENEMY_RHOMBUS();

	/* Create Animations and Connect them with Transitions */
	std::shared_ptr<Animation> moving = std::make_shared<Animation>(Resources::Graphics::ENEMY_RHOMBUS_NORMAL);
	std::shared_ptr<Animation> damaged = std::make_shared<Animation>(Resources::Graphics::ENEMY_RHOMBUS_DAMAGED);
	std::shared_ptr<Animation> warning = std::make_shared<Animation>(Resources::Graphics::ENEMY_RHOMBUS_WARNING);
	std::shared_ptr<Animation> spawning = std::make_shared<Animation>(Resources::Graphics::ENEMY_RHOMBUS_SPAWNING);

	damaged->AddTransition(Transition(moving));
	spawning->AddTransition(Transition(moving));
	moving->AddTransition(Transition(Property::OBJECT_STATE, static_cast<int>(DAMAGED), damaged));
	warning->AddTransition(Transition(Property::OBJECT_STATE, static_cast<int>(NORMAL), spawning));

	rhombus._graphicsController->SetCurrentAnimation(warning);

	/* Set Initial Properties */
	rhombus._propertyController.SetPropertyValue(Property::HEALTH, health);
	rhombus._propertyController.SetPropertyValue(Property::OBJECT_STATE, static_cast<int>(SPAWNING));

	/* Set Enemy Specific Responses */
	rhombus.SetEnemyState(EnemyStateType::NORMAL, std::make_shared<RhombusNormalState>());
	rhombus._destructionParticlesPrototype = std::make_shared<TriangleExplosion>(1);

	_enemyMappings.insert({ ObjectId::ENEMY_RHOMBUS, std::move(rhombus) });
}

void EnemyManager::ConstructSquarePrototype()
{
	const float elasticity = 0, mass = 50, speed = 1400;
	const int health = 60, width = 128, height = 128;

	Enemy square;
	/* Setup Transform Properties */
	square._transform->SetSize(width, height);
	square._transform->_RigidBody.SetElasticityCoefficient(elasticity);
	square._transform->_RigidBody.SetInvertedMass(0);
	square._transform->_RigidBody.SetSpeed(speed);
	square._transform->_Collider = Resources::Colliders::ENEMY_SQUARE();

	/* Create Animations and Connect them with Transitions */
	std::shared_ptr<Animation> moving = std::make_shared<Animation>(Resources::Graphics::ENEMY_SQUARE_NORMAL);
	std::shared_ptr<Animation> damaged = std::make_shared<Animation>(Resources::Graphics::ENEMY_SQUARE_DAMAGED);
	std::shared_ptr<Animation> warning = std::make_shared<Animation>(Resources::Graphics::ENEMY_SQUARE_WARNING);
	std::shared_ptr<Animation> spawning = std::make_shared<Animation>(Resources::Graphics::ENEMY_SQUARE_SPAWNING);

	damaged->AddTransition(Transition(moving));
	spawning->AddTransition(Transition(moving));
	moving->AddTransition(Transition(Property::OBJECT_STATE, static_cast<int>(DAMAGED), damaged));
	warning->AddTransition(Transition(Property::OBJECT_STATE, static_cast<int>(NORMAL), spawning));

	square._graphicsController->SetCurrentAnimation(warning);

	/* Set Initial Properties */
	square._propertyController.SetPropertyValue(Property::OBJECT_STATE, static_cast<int>(SPAWNING));
	square._propertyController.SetPropertyValue(Property::HEALTH, health);

	/* Set Enemy Specific Responses */
	square.SetEnemyState(EnemyStateType::NORMAL, std::make_shared<SquareNormalState>());
	square._destructionParticlesPrototype = std::make_shared<TriangleExplosion>(1.2);

	_enemyMappings.insert({ ObjectId::ENEMY_SQUARE, std::move(square) });
}

void EnemyManager::ConstructHexagonPrototype()
{
	const float _elasticity = 0, _mass = 50, _speed = 1700;
	const int _health = 90, width = 192, height = 167;

	Enemy hexagon;
	/* Setup Transform Properties */
	hexagon._transform->SetSize(width, height);
	hexagon._transform->_RigidBody.SetElasticityCoefficient(_elasticity);
	hexagon._transform->_RigidBody.SetInvertedMass(1 / _mass);
	hexagon._transform->_RigidBody.SetSpeed(_speed);
	hexagon._transform->_Collider = Resources::Colliders::ENEMY_HEXAGON();

	/* Create Animations and Connect them with Transitions */
	std::shared_ptr<Animation> moving = std::make_shared<Animation>(Resources::Graphics::ENEMY_HEXAGON_NORMAL);
	std::shared_ptr<Animation> damaged = std::make_shared<Animation>(Resources::Graphics::ENEMY_HEXAGON_DAMAGED);
	std::shared_ptr<Animation> warning = std::make_shared<Animation>(Resources::Graphics::ENEMY_HEXAGON_WARNING);
	std::shared_ptr<Animation> spawning = std::make_shared<Animation>(Resources::Graphics::ENEMY_HEXAGON_SPAWNING);

	damaged->AddTransition(Transition(moving));
	spawning->AddTransition(Transition(moving));
	moving->AddTransition(Transition(Property::OBJECT_STATE, static_cast<int>(DAMAGED), damaged));
	warning->AddTransition(Transition(Property::OBJECT_STATE, static_cast<int>(NORMAL), spawning));

	hexagon._graphicsController->SetCurrentAnimation(warning);

	/* Set Initial Properties */
	hexagon._propertyController.SetPropertyValue(Property::HEALTH, _health);
	hexagon._propertyController.SetPropertyValue(Property::OBJECT_STATE, static_cast<int>(SPAWNING));

	/* Set Enemy Specific Responses */
	hexagon.SetEnemyState(EnemyStateType::NORMAL, std::make_shared<HexagonNormalState>());
	hexagon._destructionParticlesPrototype = std::make_shared<TriangleExplosion>(1.5);

	_enemyMappings.insert({ ObjectId::ENEMY_HEXAGON, std::move(hexagon) });
}