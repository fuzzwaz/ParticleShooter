#include "EnemyBasicShot.h"
#include "PlayerInfo.h"
#include "RhombusNormalState.h"
#include "Transform.h"

#include <algorithm>

RhombusNormalState::RhombusNormalState()
{
	const auto basicAttack = std::make_shared<EnemyBasicShot>();
	_projectileShooter = std::make_unique<ProjectileShooter>(basicAttack);
}

RhombusNormalState::RhombusNormalState(const RhombusNormalState& source) : EnemyNormalState(source)
{
	_projectileShooter = std::make_unique<ProjectileShooter>(*(source._projectileShooter));
}

std::shared_ptr<EnemyState> RhombusNormalState::Clone() const
{
	return std::make_shared<RhombusNormalState>(*this);
}

/*
	Description:
		Randomizes movement and attack information to keep each Rhombus varied from the other ones.
		This happens often even with the Same Rhombus so that they are always a bit unpredictable

	Arguments:
		observerController - Unused in this derived implementation. 
		transform - Helpful in determining how to randomize movement
		graphics -  Unused in this derived implementation.
		properties -  Unused in this derived implementation.

*/
void RhombusNormalState::OnStart(ObserverController& observerController, TransformPt& transform, GControllerPt& graphics, PController& properties)
{
	EnemyNormalState::OnStart(observerController, transform, graphics, properties);
	_minimumSpeed = transform->_RigidBody.GetSpeed();

	RandomizeMovementTarget(transform);
	ResetAttackTimer();
}

/*
	Description:
		Relies on base implementation to apply damage. To better convey that a Rhombus
		it hurt, applies an impulse velocity in the Nudge Direction with a magnitude of _damagedImpulseMagnitude

	Arguments:
		pendingDamage - Used to determine if the impulse velocity should be applied
		transform - Has collision information needed to apply the impulse velocity
		properties -  Unused in this derived implementation.

*/
void RhombusNormalState::ApplyPendingDamage(const int& pendingDamage, TransformPt& transform, PController& properties)
{
	EnemyNormalState::ApplyPendingDamage(pendingDamage, transform, properties);

	if (pendingDamage > 0)
	{
		const auto info = transform->_Collider.GetCollisionResponseInfo();
		transform->_RigidBody.AddImpulseVelocity(info._NudgeDirection * _damagedImpulseMagnitude);
	}
}

/*
	Description:
		If the _attackTimer elapses _attackTime, spawns a bullet hurdling towards the Player.
		Resets the _attackTimer and randomizes _attackTime to keep things unpredictable. 

	Arguments:
		playerInfo - Needed to determine the direction of the bullet
		transform - Needed to determine bullet direction and bullet spawn position

*/
void RhombusNormalState::PerformAttackBehavior(const PlayerInfo& playerInfo, TransformPt& transform)
{
	if (_attackTimer.GetSeconds() >= _attackTime)
	{
		const Vector2 direction = playerInfo._CurrentPosition - transform->GetOrigin();
		_projectileShooter->Shoot(transform->GetOrigin(), direction.Normal());

		ResetAttackTimer();
	}
}

/*
	Description:
		Rhombus specific movement behavior. Randomizes movement properties to keep things unpredictable.
		Always faces the player, accelerates towards the player if too far away

	Arguments:
		observerController - Unused in this derived implementation. 
		playerInfo - Needed to determine forward vector, acceleration, and position details 
		transform - What is actually updating to manage movement
*/
void RhombusNormalState::PerformMovementBehavior(ObserverController& observerController, const PlayerInfo& playerInfo, TransformPt& transform)
{
	/* If colliding with anything, randomize movement. This is purely for design purposes. It keeps enemies whose paths collide from continuously colliding */
	const auto info = transform->_Collider.GetCollisionResponseInfo();
	if (info._IsColliding)
		RandomizeMovementTarget(transform);

	/* Face the Player */
	const Vector2 playerVector = playerInfo._CurrentPosition - transform->GetOrigin();
	transform->SetForwardVector(playerVector.Normal());

	const float distanceFromPlayer = playerVector.Magnitude();
	if (distanceFromPlayer > _distanceFromPlayerToMaintain) //Check to see if the enemy is close enough to the player
	{
		const Vector2 playerOffsetPosition = playerInfo._CurrentPosition + _playerOffset; //Calculate an offset position from the player to move towards. This keeps all of the enemies from heading to the exact same point
		const Vector2 acclerationDirection = playerOffsetPosition - transform->GetOrigin();
		transform->_RigidBody.SetAcceleration(acclerationDirection.Normal() * (_accelerationMagnitude)); //Accelerate in that direction
	}
	else //If it is, halt movement entirely
	{
		transform->_RigidBody.SetAcceleration(Vector2(0,0));
		transform->_RigidBody.SetInputVelocity(Vector2(0, 0));
	}

	EnemyNormalState::PerformMovementBehavior(observerController, playerInfo, transform);
}

/*
	Description:
		Instead of heading directly towards the Player, Rhombus head towards a position that is
		slightly offset from the Player. This is useful when there are dozens on the screen and you don't
		want them to all move in exactly the same way.

		This function calculates that randomized offset as well as calculates a randomized speed change so that
		all Rhombuses are not moving at the same speed.

	Arguments:
		transform - Updates the speed on the transform with the new randomized speed
*/
void RhombusNormalState::RandomizeMovementTarget(TransformPt& transform)
{
	_playerOffset = CommonHelpers::RandomOffset(_playerOffsetMagnitude);
	const Vector2 speedOffset = CommonHelpers::RandomOffset(_speedOffsetMagnitude);
	transform->_RigidBody.SetSpeed(_minimumSpeed + speedOffset.x);
}

/*
	Description:
		Randomly selects a MS amount according to _attackTimeOffsetMagnitude. Sets this amount
		as the next attack time. Restarts the timer.
*/
void RhombusNormalState::ResetAttackTimer()
{
	const int randomMillisecond = (rand() % _attackTimeOffsetMagnitude);
	const float randomSeconds = randomMillisecond / 1000.0;
	_attackTime = std::fmax(randomSeconds, 1.0);
	_attackTimer.Start();
}