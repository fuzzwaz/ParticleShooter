#include "HexagonNormalState.h"
#include "HexBeam.h"
#include "Transform.h"

/*
	Description:
		Constructs everything needed for projectile shooting. The projectile being shot (HexBeam),
		the projectile shooter itself, and the list of locations to spawn the projectiles.

		The projectile locations align with the edges of the Hexagon in the two different rotational orientation aliases
		(referred to as "Standard" and "Rotated")
*/
HexagonNormalState::HexagonNormalState()
{
	const auto hexBeam = std::make_shared<HexBeam>();
	_projectileShooter = std::make_unique<ProjectileShooter>(hexBeam, false);
	_standardProjectileSpawnLocations = std::make_shared<std::vector<Vector2>>();
	_standardProjectileSpawnLocations->insert(_standardProjectileSpawnLocations->begin(),
		{ Vector2(-58.5, -35), Vector2(-58.5, 35), Vector2(58.5, -35), Vector2(58.5, 35), Vector2(0, 83.5), Vector2(0, -83.5) });
	_rotatedProjectileSpawnLocations = std::make_shared<std::vector<Vector2>>();
	_rotatedProjectileSpawnLocations->insert(_rotatedProjectileSpawnLocations->begin(),
		{ Vector2(-48, -83.5), Vector2(-48, 83.5), Vector2(48, -83.5), Vector2(48, 83.5), Vector2(-96, 0), Vector2(96, 0) });

	_attackTimer.Start();
}

HexagonNormalState::HexagonNormalState(const HexagonNormalState& source) : EnemyNormalState(source)
{
	_projectileShooter = std::make_unique<ProjectileShooter>(*(source._projectileShooter));
	_standardProjectileSpawnLocations = source._standardProjectileSpawnLocations;
	_rotatedProjectileSpawnLocations = source._rotatedProjectileSpawnLocations;

	_attackTimer.Start();
}

std::shared_ptr<EnemyState> HexagonNormalState::Clone() const
{
	return std::make_shared<HexagonNormalState>(*this);
}

EnemyStateType HexagonNormalState::Update(const PlayerInfo& playerInfo, ObserverController& observerController, TransformPt& transform, GControllerPt& graphics, PController& properties)
{
	/*
		Update animation so that the final frame syncs with the attack timer.
		This gives the player a heads up when to expect an incoming attack
	 */
	const float lerpCompletionPercentage = (_attackTimer.GetSeconds() / _attackTime) * 100;
	graphics->SetAnimationFramePercent(lerpCompletionPercentage);

	return EnemyNormalState::Update(playerInfo, observerController, transform, graphics, properties);
}

/*
	Description:
		Linearly interpolates the Hexagon's rotation from _currentAngle to _nextAngle.
		_attackTime determines how far along the interpolation has gone.

	Arguments:
		observerController - Unused in this derived implementation
		playerInfo - Unused in this derived implementation
		transform - Allows us to set the current orientation angle to match the lerp increment
*/
void HexagonNormalState::PerformMovementBehavior(ObserverController& observerController, const PlayerInfo& playerInfo, TransformPt& transform)
{
	const float lerpValue = (_attackTimer.GetSeconds() / _attackTime);
	const float remainingAngleToTravel = _nextAngle - _currentAngle;
	const float nextAngleIncrement = _currentAngle + (remainingAngleToTravel * lerpValue);

	transform->SetOrientationAngle(nextAngleIncrement);

	EnemyNormalState::PerformMovementBehavior(observerController, playerInfo, transform);
}

/*
	Description:
		When the _attackTimer elapses _attackTime, Hexagon shoots out projectiles at each of their edges.
		_rotated tells us which list of spawnLocations to use. Use those spawnLocations to spawn the projectiles.

		Prepares for the next attack by resetting the timer and setting the _nextAngle to rotate towards

	Arguments:
		playerInfo - Unused in this derived implementation
		transform - Used to give use the position to spawn the projectiles from
*/
void HexagonNormalState::PerformAttackBehavior(const PlayerInfo& playerInfo, TransformPt& transform)
{
	if (_attackTimer.GetSeconds() >= _attackTime)
	{
		if (!_rotated)
		{
			Shoot(transform, _standardProjectileSpawnLocations);
			_rotated = true;
		}
		else
		{
			Shoot(transform, _rotatedProjectileSpawnLocations);
			_rotated = false;
		}

		_currentAngle = transform->GetOrientationAngle();
		_nextAngle = _currentAngle + _rotationPerAttack;
		_attackTimer.Start();
	}
}

/*
	Description:
		Goes through a list of Vector2 locations and shoots at that position and direction
*/
void HexagonNormalState::Shoot(const TransformPt& transform, const std::shared_ptr<const std::vector<Vector2>>& spawnLocations) const
{
	for (const Vector2& spawnLocation : *spawnLocations)
	{
		_projectileShooter->Shoot(transform->GetOrigin() + spawnLocation, spawnLocation.Normal());
	}
}