#include "InputState.h"
#include "Player.h"
#include "PlayerBounceShot.h"
#include "PlayerStateBounce.h"

PlayerStateBounce::PlayerStateBounce()
{
    static const auto bounceAttack = std::make_shared<PlayerBounceShot>();
    _projectileShooter = std::make_unique<ProjectileShooter>(bounceAttack);
}

/*
    Description:
        Prepares the Player object for the Bounce Power Up State

    Arguments:
        propController - Property controller which stores which power ups are active on the player
        transform - Position and collision information
        observerController - To Notify the objects observing the Player
*/
void PlayerStateBounce::OnStart(PropertyController& propController, const std::shared_ptr<Transform>& transform, const ObserverController& observerController)
{
    transform->_RigidBody.SetSpeed(_movementSpeedNormal);
    propController.SetPropertyValue(Property::POWER_ACTIVE, 1); //Toggle Bounce Power Up to be Active in the Player's property controller

    if (!_bouncePowerUpAlreadyInProgress) //Check if the duration timer has already begun before resetting it again
    {
        _bounceStateDurationTimer.Start();
        _bouncePowerUpAlreadyInProgress = true;
    }
}

/*
    Description:
        Updates the Player's movement, attacks, and player properties based on current input and collision information.
        Manages the duration time of the Bounce power up. Resets the timer if colliding with another power up. Returns
        to a Normal Player State if the duration is complete

    Arguments:
        transform - Position and collision information
        propController - Property controller which stores which power ups are active on the player
        inputState - Current frame's input information
        observerController - To Notify the objects observing the Player

    Returns:
        PlayerStateType - The next PlayerState to switch to. Defaults to the Bounce State until the power up's duration is up
*/
PlayerStateType PlayerStateBounce::Update(std::shared_ptr<Transform> transform, PropertyController& propController, const InputState& inputState, const ObserverController& observerController)
{
    const CollisionResponseInfo collisionInfo = ResolveCollisions(transform, propController, observerController);
    UpdateMovement(inputState, transform);
    UpdateAttacks(propController, inputState, transform);

    if (CollidingWithPickUp(collisionInfo, ObjectId::PICK_UP_BOUNCE)) //If the Player picks up another bounce power up, reset the timer
        _bounceStateDurationTimer.Start();

    if (_tookDamage)
    {
        _tookDamage = false;
        return PlayerStateType::DAMAGED;
    }
    else if (_bounceStateDurationTimer.GetSeconds() >= _bounceStateDurationTime)
    {
        _bouncePowerUpAlreadyInProgress = false;
        observerController.NotifyObservers(GameObjectEvent::POWERED_DOWN);
        return PlayerStateType::NORMAL;
    }
    else if (inputState._BeamPressed && HasSpecialPowerAvailable(propController))
        return PlayerStateType::LASER;
    else
		return PlayerStateType::BOUNCE;
}