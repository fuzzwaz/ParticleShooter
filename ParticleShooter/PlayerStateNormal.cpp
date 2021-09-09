#include "Projectile.h"
#include "Common.h"
#include "InputState.h"
#include "GameObjectProperties.h"
#include "Player.h"
#include "PlayerStateNormal.h"

#include <algorithm>


PlayerStateNormal::PlayerStateNormal()
{
    static const auto basicAttack = std::make_shared<Projectile>();
    _projectileShooter = std::make_unique<ProjectileShooter>(basicAttack); //By default, the Player shoots "BasicShot"s
}

void PlayerStateNormal::OnStart(PropertyController& propController, const std::shared_ptr<Transform>& transform, const ObserverController& observerController)
{
    transform->_RigidBody.SetSpeed(_movementSpeedNormal);
    propController.SetPropertyValue(Property::POWER_ACTIVE, 0); //Normal Player State implies that a power up is not active
}

/*
    Description:
        Updates the Player's movement, attacks, and player properties based on current input and collision information

    Arguments:
        transform - Position and collision information
        propController - Properties including health and special ability power
        inputState - Current frame's input information
        observerController - To Notify the objects observing the Player

    Returns:
        PlayerStateType - The next PlayerState to switch to
*/
PlayerStateType PlayerStateNormal::Update(std::shared_ptr<Transform> transform, PropertyController& propController, const InputState& inputState, const ObserverController& observerController)
{
	const CollisionResponseInfo collisionInfo = ResolveCollisions(transform, propController, observerController);
    UpdateMovement(inputState, transform);
    UpdateAttacks(propController, inputState, transform);

    if (_tookDamage)
    {
        _tookDamage = false;
        return PlayerStateType::DAMAGED;
    }
    else if (CollidingWithPickUp(collisionInfo, ObjectId::PICK_UP_BOUNCE))
        return PlayerStateType::BOUNCE;
	else if (inputState._BeamPressed && HasSpecialPowerAvailable(propController))
        return PlayerStateType::LASER;

    return PlayerStateType::NORMAL;
}

/*
    Description:
        Applies collision response vectors to the Player's transforms. Update's the Player State and Properties based on what
        the Playeyr is currently colliding with

    Arguments:
        transform - Position and collision information
        propController - Properties including health and special ability power
        observerController - To Notify the objects observing the Player

    Returns:
        CollisionResponseInfo - An information struct about the the collisions that were resolved
*/
CollisionResponseInfo PlayerStateNormal::ResolveCollisions(std::shared_ptr<Transform>& transform, PropertyController& propController, const ObserverController& observerController)
{
    const CollisionResponseInfo collisionInfo = transform->_Collider.GetCollisionResponseInfo();
    const std::vector<ColliderBaton> batons = collisionInfo._Batons;

    /* Accumulating health changes based on all colliding objects  */
    int damageTaken = 0, healthGained = 0;
    for (const ColliderBaton& baton : batons)
    {
        if (baton._Type == ColliderType::ENEMYATTACK || baton._Type == ColliderType::ENEMY)
            damageTaken += baton._Value;
        else if (baton._Type == ColliderType::PICKUP && baton._Id == ObjectId::PICK_UP_HEALTH)
            healthGained += baton._Value;
    }

    IncreaseHealth(propController, healthGained);
    TakeDamage(propController, damageTaken);
    transform->ResolveCollisions();

    return std::move(collisionInfo);
}

/*
    Description:
        Updates the Player's movement by applying acceleration vectors based on the current input

    Arguments:
    transform - Position and collision information
    inputState - Current frame's input information
*/
void PlayerStateNormal::UpdateMovement(const InputState& inputState, const std::shared_ptr<Transform>& transform) const
{
    if (inputState._MovementDirection.x != 0 || inputState._MovementDirection.y != 0)
        transform->SetForwardVector(inputState._MovementDirection);
    
    transform->_RigidBody.SetAcceleration(inputState._MovementDirection.Normal() * _movementAcclerationMagnitude);
    const Vector2 playerDisplacementAfterAcceleration = transform->_RigidBody.ApplyMovementForces();
    transform->Move(playerDisplacementAfterAcceleration);
}

/*
    Description:
        Spawns projectile bullets based on the current input and refills the special ability bar.

    Arguments:
        propController - Properties including health and special ability power
        inputState - Current frame's input information
        transform - Position and collision information
*/
void PlayerStateNormal::UpdateAttacks(PropertyController& propController, const InputState& inputState, const std::shared_ptr<const Transform>& transform)
{
    if (inputState._ShootPressed)
    {
        Vector2 shotDirection = Vector2(inputState._CursorPosition.x - transform->GetOrigin().x, inputState._CursorPosition.y - transform->GetOrigin().y);
        shotDirection.Normalize();
        const Vector2 shotPosition = transform->GetOrigin() + (shotDirection * _projectileSpawnDistance);
        
        _projectileShooter->Shoot(shotPosition, shotDirection);
    }

    RefillSpecialAbilityBar(propController);
}

#pragma region Helper Functions

void PlayerStateNormal::IncreaseHealth(PropertyController& propController, const int& health) const
{
    if (health > 0)
    {
    	const int currentHealth = propController.GetPropertyValue(Property::HEALTH);
        const int newHealth = std::min(_maxHealth, currentHealth + health);
        propController.SetPropertyValue(Property::HEALTH, newHealth);
    }
}

void PlayerStateNormal::TakeDamage(PropertyController& propController, const int& damage)
{
    if (damage > 0)
    {
        const int currentHealth = propController.GetPropertyValue(Property::HEALTH);
        const int newHealth = std::max(0, currentHealth - damage);
        propController.SetPropertyValue(Property::HEALTH, newHealth);
        _tookDamage = true;
    }
}

void PlayerStateNormal::RefillSpecialAbilityBar(PropertyController& propController)
{
	const int currentSpecialPower = propController.GetPropertyValue(Property::SPECIAL_POWER);

    if (currentSpecialPower < _maxSpecialAbilityPower && !_specialAbilityRefillTimer.GetActive())
    {
        _specialAbilityRefillTimer.Start();
    }
    else if (currentSpecialPower < _maxSpecialAbilityPower && _specialAbilityRefillTimer.GetSeconds() >= _specialAbilityRefillTime) //Enough time has passed to refill more special ability power
    {
        const int newSpecialAmount = std::min(currentSpecialPower + _specialAbilityRefillAmount, _maxSpecialAbilityPower);
        propController.SetPropertyValue(Property::SPECIAL_POWER, newSpecialAmount);
        _specialAbilityRefillTimer.Start();
    }
    else if (currentSpecialPower >= _maxSpecialAbilityPower && _specialAbilityRefillTimer.GetActive()) //Ability power bar is fully filled
    {
        _specialAbilityRefillTimer.Stop();
    }
}

bool CollidingWithPickUp(const CollisionResponseInfo& collisionInfo, const ObjectId& pickUpId)
{
    auto collidingWithPickUp = std::find_if(collisionInfo._Batons.cbegin(), collisionInfo._Batons.cend(), [pickUpId](const ColliderBaton& baton)
        { return baton._Type == ColliderType::PICKUP && baton._Id == pickUpId; });

    return collidingWithPickUp != collisionInfo._Batons.cend();
}

#pragma endregion
