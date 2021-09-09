#include "Animation.h"
#include "Enemy.h"
#include "EnemyDamagedState.h"
#include "EnemySpawningState.h"
#include "EnemyState.h"
#include "ItemSpawner.h"
#include "PlayerInfo.h"
#include "Transform.h"


Enemy::Enemy() : GraphicObject()
{
	_stateMappings.insert({ EnemyStateType::NORMAL, std::make_shared<EnemyNormalState>() });
	_stateMappings.insert({ EnemyStateType::DAMAGED, std::make_shared<EnemyDamagedState>() });
	_stateMappings.insert({ EnemyStateType::SPAWNING, std::make_shared<EnemySpawningState>() });

	_state = _stateMappings[EnemyStateType::SPAWNING];
}

/*
	Description:
		Copies Enemy specific properties from the source. Calls base constructors as well to copy other propertes.
		Enemies can differ by their EnemyState implementation so we need to go through all of the source's states
		and clone them in order to successfully copy.

	Arguments:
		source - The eEnemy being copied
*/
Enemy::Enemy(const Enemy& source) : GraphicObject(source)
{
	_destructionParticlesPrototype = source._destructionParticlesPrototype;
	_transform->_Collider.SetAssociatedRigidBody(&_transform->_RigidBody);

	for (auto it = source._stateMappings.cbegin(); it != source._stateMappings.cend(); ++it)
	{
		_stateMappings.insert({ (*it).first , (*it).second->Clone() });
	}

	_state = _stateMappings[EnemyStateType::SPAWNING];
}

std::shared_ptr<Enemy> Enemy::Clone(const Vector2& position) const
{
	std::shared_ptr<Enemy> enemyClone = std::make_shared<Enemy>(*this);
	enemyClone->SetPosition(position);
	enemyClone->SetIsActive(true);
	enemyClone->StartCurrentState();

	return enemyClone;
}

/*
	Description:
		Pairs an EnemyStateType to a specific EnemyState implementation.
		Adds a new State to _stateMappings if one is not already found. Otherwise, overwrites.

	Arguments:
		type - EnemyStateType. Becomes paired with state in _stateMappings
		state - EnemyState. Becomes paired with the type in _stateMappings
*/
void Enemy::SetEnemyState(const EnemyStateType& type, const std::shared_ptr<EnemyState>& state)
{
	const auto foundIt = _stateMappings.find(type);

	if (foundIt == _stateMappings.end())
		_stateMappings.insert({ type, state });
	else
		_stateMappings[type] = state;
}

void Enemy::StartCurrentState()
{
	_state->OnStart(_observerController, _transform, _graphicsController, _propertyController);
}

/*
	Description:
		Calls Update on the active EnemyState.
		Manages State Switching when the current State suggests a new State.
		Destroys the Enemy when DESTROYED type gets suggested.

	Arguments:
		playerInfo - Player properties for the current frame. Passed into the EnemyState for evaluation.
		cameraPosition - Camera's World Coordinates for the current frame. Passed into the EnemyState for evaluation.
		input - System's input read for the current frame. Passed into the EnemyState for evaluation.
*/
void Enemy::Update(const PlayerInfo& playerInfo, const Vector2& cameraPosition, const InputState& input)
{
	EnemyStateType nextState = _state->Update( playerInfo, _observerController, _transform, _graphicsController, _propertyController);

	if (nextState == EnemyStateType::DESTROYED)
	{
		Destroyed();
		SetIsActive(false);
		return;
	}
	else if (nextState != _state->GetStateType())
	{
		SwitchEnemyState(nextState);
	}

	_graphicsController->Update();
}

/*
	Description:
		Passes the torch from one EnemyState to the next. First calls "OnEnd" on current state.
		Then uses _stateMappings to set the next state as the current state.
		Then calls "OnStart" for this new state.

	Arguments:
		type - The EnemyStateType which pairs with an EnemyState implementation. This implementation will become the next active state
*/
void Enemy::SwitchEnemyState(const EnemyStateType& type)
{
	_state->OnEnd();

	const auto foundIt = _stateMappings.find(type);
	if (foundIt == _stateMappings.end())
		_state = _stateMappings.at(EnemyStateType::NORMAL);
	else
		_state = _stateMappings[type];

	_propertyController.SetPropertyValue(Property::OBJECT_STATE, type);
	_state->OnStart(_observerController, _transform, _graphicsController, _propertyController);
}

ColliderInterface* Enemy::GetCollider()
{
	return &_transform->_Collider;
}

void Enemy::Destroyed()
{
	_observerController.NotifyObservers(GameObjectEvent::ENEMY_DEATH);
	ItemSpawner::GetInstance().SpawnItem(_transform->GetOrigin());
	SpawnGraphicObject(*_destructionParticlesPrototype, _transform->GetOrigin());
	_observerController.NotifyObservers(GameObjectEvent::DESTROYED);
}