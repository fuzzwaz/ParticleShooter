#include "Enemy.h"
#include "EnemyDamagedState.h"
#include "Transform.h"

std::shared_ptr<EnemyState> EnemyDamagedState::Clone() const
{
	return std::make_shared<EnemyDamagedState>();
}

void EnemyDamagedState::OnStart(ObserverController& observerController, TransformPt& transform, GControllerPt& graphics, PController& properties)
{
	_damagedStateTimer.Start();
}

/*
	Description:
		Responds to any Collisions and preexisting movement velocities.
		Checks to see if it's time to leave the Damaged State

	Arguments:
		playerInfo - Unused in this derived implementation
		observerController - Unused in this derived implementation
		transform - Handles movement and collision response
		graphics - Unused in this derived implementation
		properties - Unused in this derived implementation
*/
EnemyStateType EnemyDamagedState::Update(const PlayerInfo& playerInfo, ObserverController& observerController, TransformPt& transform, GControllerPt& graphics, PController& properties)
{
	transform->ResolveCollisions();
	const Vector2 displacement = transform->_RigidBody.ApplyMovementForces();
	transform->Move(displacement);

	if (_damagedStateTimer.GetSeconds() > _damagedStateTime)
		return EnemyStateType::NORMAL;

	return EnemyStateType::DAMAGED;
}