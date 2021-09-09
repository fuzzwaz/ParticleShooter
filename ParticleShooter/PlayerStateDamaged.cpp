#include "PlayerStateDamaged.h"
#include "GameObjectObserver.h"
#include "GameObjectProperties.h"
#include "ObserverController.h"

/*
	Description:
		Notifies the Player's properties and observers that the player has been damaged and is entering a
		damaged state

	Arguments:
		propController - Property controller which stores information about whether the player is damaged or not
		transform - Position and collision information
		observerController - To Notify the objects observing the Player
*/
void PlayerStateDamaged::OnStart(PropertyController& propController, const std::shared_ptr<Transform>& transform, const ObserverController& observerController)
{
	_damagedTimer.Start();
	observerController.NotifyObservers(GameObjectEvent::PLAYER_DAMAGE, true);
	propController.SetPropertyValue(Property::IS_DAMAGED, true);

	//If the player has no health, signal a Game Over to observers
	if (propController.GetPropertyValue(Property::HEALTH) <= 0)
		observerController.NotifyObservers(GameObjectEvent::GAME_OVER);
}

/*
	Description:
		Updates the Player's movement, attacks, and player properties based on current input and collision information.
		Works identically to the Normal Player State while preventing damage for the duration of "_damagedTime"

	Arguments:
		transform - Position and collision information
		propController - Property controller which stores which power ups are active on the player
		inputState - Current frame's input information
		observerController - To Notify the objects observing the Player

	Returns:
		PlayerStateType - The next PlayerState to switch to. Defaults to the Damaged State until the "_damagedTime" duration is up
*/
PlayerStateType PlayerStateDamaged::Update(std::shared_ptr<Transform> transform, PropertyController& propController, const InputState& inputState, const ObserverController& observerController)
{
	PlayerStateType nextState = PlayerStateNormal::Update(transform, propController, inputState, observerController);

	if (_damagedTimer.GetSeconds() > _damagedTime)
	{
		_damagedTimer.Stop();
		observerController.NotifyObservers(GameObjectEvent::PLAYER_DAMAGE, false);
		propController.SetPropertyValue(Property::IS_DAMAGED, false);

		if ( nextState == PlayerStateType::NORMAL && propController.ComparePropertyValue(Property::POWER_ACTIVE, 1))
			return PlayerStateType::BOUNCE;

		return nextState;
	}

	return PlayerStateType::DAMAGED;
}
