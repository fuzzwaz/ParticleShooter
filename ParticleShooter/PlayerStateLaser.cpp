#include "GameObjectRegistryLocator.h"
#include "GameObjectRegistryService.h"
#include "InputState.h"
#include "Laser.h"
#include "PlayerStateLaser.h"
#include "Transform.h"

#include <algorithm>

PlayerStateLaser::PlayerStateLaser()
{
	_laser.reset(new Laser());
	GameObjectRegistryLocator::GetCreatorService()->AddGameObjectToScene(_laser);
}

void PlayerStateLaser::OnStart(PropertyController& propController, const std::shared_ptr<Transform>& transform, const ObserverController& observerController)
{
	transform->_RigidBody.SetSpeed(_movementSpeedSpecial);
	_specialAbilityDepleteTimer.Start();
}

/*
	Description:
		Updates the Player's movement, attacks, and player properties based on current input and collision information.
		Manages the Player's special ability power. Depletes the power while in the laser state. Once the power is fully
		depleted, suggests a new Player State.

	Arguments:
		transform - Position and collision information
		propController - Property controller which stores the player's current special ability power
		inputState - Current frame's input information
		observerController - To Notify the objects observing the Player

	Returns:
		PlayerStateType - The next PlayerState to switch to
*/
PlayerStateType PlayerStateLaser::Update(std::shared_ptr<Transform> transform, PropertyController& propController, const InputState& inputState, const ObserverController& observerController)
{
	DepleteSpecialAbilityBar(propController);

	if (!inputState._BeamPressed || propController.GetPropertyValue(Property::SPECIAL_POWER) <= 0)
	{
		if (propController.ComparePropertyValue(Property::POWER_ACTIVE, 1))
			return PlayerStateType::BOUNCE;
		else
			return PlayerStateType::NORMAL;
	}

	ResolveCollisions(transform, propController, observerController);
	UpdateMovement(inputState, transform);
	return PlayerStateType::LASER;
}

/*
	Description:
		Depletes the Player's special ability power by _specialAbilityDepleteAmount every _specialAbilityDepleteTime seconds

	Arguments:
		propController - Property controller which stores the player's current special ability power
*/
void PlayerStateLaser::DepleteSpecialAbilityBar(PropertyController& propController)
{
	if (_specialAbilityDepleteTimer.GetSeconds() > _specialAbilityDepleteTime)
	{
		const int specialPower = propController.GetPropertyValue(Property::SPECIAL_POWER);
		const int newSpecialAmount = std::max(specialPower - _specialAbilityDepleteAmount, 0);
		propController.SetPropertyValue(Property::SPECIAL_POWER, newSpecialAmount);

		_specialAbilityDepleteTimer.Start();
	}
}