#include "Enemy.h"
#include "GraphicAssetResources.h"
#include "GraphicObject.h"
#include "PlayerInfo.h"
#include "SquareNormalState.h"
#include "Transform.h"

SquareNormalState::SquareNormalState()
{
	_attackWarningBeamPrototype = std::make_shared<GraphicObject>(Resources::Graphics::WARNING_BEAM_EFFECT, 0.5);
}

SquareNormalState::SquareNormalState(const SquareNormalState& source) : EnemyNormalState(source)
{
	_attackWarningBeamPrototype = source._attackWarningBeamPrototype;
}

std::shared_ptr<EnemyState> SquareNormalState::Clone() const
{
	return std::make_shared<SquareNormalState>(*this);
}

/*
	Description:
		Transitioning timers and properties from the Attack Stage to the Time Out Stage
*/
void SquareNormalState::FinishAttacking()
{
	_attacking = false;
	_attackTimer.Stop();
	_inactiveTimer.Start();
	_inactive = true;
}

/*
	Description:
		Transitioning timers and properties from the Time Out Stage to the Searching for Targets Stage
*/
void SquareNormalState::FinishInactiveTimeOut()
{
	_inactive = false;
	_inactiveTimer.Stop();
}

/*
	Description:
		Transitioning timers and properties from the Warning Stage to the Attack Stage
*/
void SquareNormalState::FinishTargetWarning()
{
	_warningTimer.Stop();
	_warningInitiated = false;
	_attacking = true;
	_attackTimer.Start();
}

/*
	Description:
		The first stage of the Square's movement behavior. Checks to see if the Player in within range
		of the Square. If so, ends the search Stage by starting the Warning Stage

	Arguments:
		playerInfo - To identify the Player's position
		transform - To identify the Square's position
		observerController -  To notify observers that a Square enemy has found a target

*/
void SquareNormalState::SearchForTargets(const PlayerInfo& playerInfo, const TransformPt& transform, ObserverController& observerController)
{
	const Vector2 PlayerVector = playerInfo._CurrentPosition - transform->GetOrigin();
	const float verticalAngle = UP.AngleBetween(PlayerVector);
	const float horizontalAngle = RIGHT.AngleBetween(PlayerVector);

	bool targetInRange = CheckIfTargetIsInRange(verticalAngle, UP, DOWN);
	if (!targetInRange)
		targetInRange = CheckIfTargetIsInRange(horizontalAngle, RIGHT, LEFT);

	if (targetInRange)
	{
		BeginWarningForUpcomingAttack(transform);
		observerController.NotifyObservers(GameObjectEvent::ENEMY_TARGET_LOCKED, true);
	}
}

/*
	Description:
		Used in the Searching Stage to compare the target's angle with the Square to its viable attack directions
		(The four cardinal directions). If the angle is small enough, it sets the attack direction as such.

		Checks both the near and far angles at the same time (if the target is behind or in front of the player)

	Arguments:
		targetAngle - Angle from one of the Square's attack directions to the Target
		closeDirection - The initial direction used to form the targetAngle
		farDirection -  The opposite direction

	Return:
		bool - Whether or not the Target was within the _targetSeekingHalfAngle in either the close or far direction

*/
bool SquareNormalState::CheckIfTargetIsInRange(const float& targetAngle, const Vector2& closeDirection, const Vector2 farDirection)
{
	if (targetAngle <= _targetSeekingHalfAngle || targetAngle >= 180 - _targetSeekingHalfAngle)
	{
		if (targetAngle <= _targetSeekingHalfAngle)
			_attackDirection = closeDirection;
		else
			_attackDirection = farDirection;

		return true;
	}
	return false;
}

/*
	Description:
		Starts the Warning Stage. Specifically, Spawns warning beams to alert
		the Player that an Attack is coming.

	Arguments:
		transform - Used to know where to spawn the warning beams
*/
void SquareNormalState::BeginWarningForUpcomingAttack(const TransformPt& transform)
{
	_warningTimer.Start();
	_warningInitiated = true;

	/* Spawn two warning beams on top of each other. A Single warning beam is not tall enough */
	const int warningBeamHeight = 1400;
	const Vector2 stackedWarningBeamLocation = transform->GetOrigin() + _attackDirection * warningBeamHeight;
	SpawnGraphicObject(*_attackWarningBeamPrototype, transform->GetOrigin() + _attackDirection, _attackDirection);
	SpawnGraphicObject(*_attackWarningBeamPrototype, stackedWarningBeamLocation, _attackDirection);
}

/*
	Description:
		Movement script for Squares. Manages the transitions from each of the 5 stages:
			Stage #1: Searching for Targets
			Stage #2: Warning
			Stage #3: Attacking
			Stage #4: Time Out
			(Repeat)

	Arguments:
		playerInfo - To identify the Player's position
		transform - To identify the Square's position
		observerController -  To notify observers that a Square enemy has found a target

*/
void SquareNormalState::PerformMovementBehavior(ObserverController& observerController, const PlayerInfo& playerInfo, TransformPt& transform)
{
	AbruptStopIfColliding(transform); //Squares abruptly stop their actions if they run into certain objects in the scene

	const bool AttackingIsComplete = _attacking && _attackTimer.GetSeconds() >= _attackTime;
	if (AttackingIsComplete)
		FinishAttacking();

	const bool InactiveTimeoutComplete = _inactive && _inactiveTimer.GetSeconds() >= _inactiveTime;
	if (InactiveTimeoutComplete)
		FinishInactiveTimeOut();

	const bool TargetWarningComplete = _warningInitiated && _warningTimer.GetSeconds() >= _warningTime;
	if (TargetWarningComplete)
		FinishTargetWarning();

	const bool IsReadyToSearchForNewTarget = !_attacking && !_inactive && !_warningInitiated;
	if (IsReadyToSearchForNewTarget)
		SearchForTargets(playerInfo, transform, observerController);

	if (_attacking)
		transform->_RigidBody.SetAcceleration(_attackDirection * _accelerationMagnitude); //Square maintains the same acceleration magnitude while attacking
	else
		transform->_RigidBody.SetAcceleration(Vector2(0, 0));

	EnemyNormalState::PerformMovementBehavior(observerController, playerInfo, transform);
}

void SquareNormalState::AbruptStopIfColliding(const TransformPt& transform)
{
	const CollisionResponseInfo colInfo = transform->_Collider.GetCollisionResponseInfo();

	//Square only abrupt stops when colliding with the environment, player, Square Enemy, or Hexagon Enemy
	const auto foundStopperIt = std::find_if(colInfo._Batons.cbegin(), colInfo._Batons.cend(),
		[](const ColliderBaton& baton)
		{
			return baton._Type == ColliderType::ENVIRONMENT || baton._Type == ColliderType::PLAYER ||
				baton._Id == ObjectId::ENEMY_SQUARE || baton._Id == ObjectId::ENEMY_HEXAGON;
		});


	//If Square is colliding with an acceptable object, abruptly end its attacking phase 
	if (foundStopperIt != colInfo._Batons.cend())
	{
		_attacking = false;
		_attackTimer.Stop();

		_inactiveTimer.Start();
		_inactive = true;

		transform->_RigidBody.SetAcceleration(Vector2(0, 0));
		transform->_RigidBody.SetInputVelocity(Vector2(0, 0));
		transform->_RigidBody.AddImpulseVelocity(colInfo._ImpulseVector * -1); //Cancel out the impulses that this collision will have in order to make it abrupt
	}
}