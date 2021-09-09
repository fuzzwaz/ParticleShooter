#include "Enemy.h"
#include "EnemyNormalState.h"
#include "GameObjectProperties.h"
#include "GraphicAssetResources.h"
#include "GraphicObject.h"
#include "Transform.h"

std::shared_ptr<EnemyState> EnemyNormalState::Clone() const
{
	return std::make_shared<EnemyNormalState>(*this);
}

void EnemyNormalState::OnStart(ObserverController& observerController, TransformPt& transform, GControllerPt& graphics, PController& properties)
{
	transform->_Collider.SetIsActive(true);
}

/*
	Description:
		Determines the Enemy's current health, runs through movement and attack behaviors,
		and suggests the next Enemy State that should be transitioned to

	Arguments:
		playerInfo - Useful when determining Attacks (locations and directions, for example)
		observerController - Keeps observers up to date about what's happening to the Enemy
		transform - Stores collision info and used to update Enemy's movement
		graphics - Unused in the Normal state.
		properties - Updated each frame with the Enemy's latest state and Health

	Return:
		EnemyStateType - Suggestion for the next State the enemy should transition to
*/
EnemyStateType EnemyNormalState::Update(const PlayerInfo& playerInfo, ObserverController& observerController, TransformPt& transform, GControllerPt& graphics, PController& properties)
{
	/* Checks for incoming damage and applies that damage to the enemy */
	const int pendingDamage = CalculatePendingDamage(transform, observerController);
	ApplyPendingDamage(pendingDamage, transform, properties);

	/* If the enemy has taken too much damage, dip out earlier and switch to the DESTROYED state */
	const int currentHealth = properties.GetPropertyValue(Property::HEALTH);
	if (currentHealth <= 0)
		return EnemyStateType::DESTROYED;

	/* If the enemy is still alive, perform movement and attack scripts */
	PerformMovementBehavior(observerController, playerInfo, transform);
	PerformAttackBehavior(playerInfo, transform);

	/* Suggest the next state that the Enemy should transition to */
	if (pendingDamage > 0)
		return EnemyStateType::DAMAGED;
	return EnemyStateType::NORMAL;
}

/*
	Description:
		Default Movement Behavior. Responds to collisions and updates movement forces.

	Arguments:
		observerController - Unused in base implementation. Keeps observers up to date about what's happening to the Enemy
		playerInfo - Unused in base implementation. Useful when determining Attacks (locations and directions, for example)
		transform - Stores collision and movement information
*/
void EnemyNormalState::PerformMovementBehavior(ObserverController& observerController, const PlayerInfo& playerInfo, TransformPt& transform)
{
	transform->ResolveCollisions();
	const Vector2 displacement = transform->_RigidBody.ApplyMovementForces();
	transform->Move(displacement);
}

/*
	Description:
		Default Attack Behavior
*/
void EnemyNormalState::PerformAttackBehavior(const PlayerInfo& playerInfo, TransformPt& transform)
{
	
}

/*
	Description:
		Combs through all colliding objects. Determines their damage magnitude and
		returns this magnitude to the caller. Notifies observers of the type of damage
		being experienced. Does not apply damage.

	Arguments:
		observerController - Tells observers whether the enemy is taking special or normal damage
		transform - Stores specific information about each collider.

	Returns:
		int - The total accumulated amount of damage that the enemy has taken according to their current collisions
*/
int EnemyNormalState::CalculatePendingDamage(const TransformPt& transform, ObserverController& observerController) const
{
	int pendingDamage = 0;
	bool playerBasicHit = false, playerSpecialHit = false;

	const auto collisionInfo = transform->_Collider.GetCollisionResponseInfo();
	/* Loop through all colliders to determine which will cause damage to the Enemy */
	for (const ColliderBaton& baton : collisionInfo._Batons)
	{
		if (baton._Type == ColliderType::PLAYERATTACK) //Only Player Attacks can hurt enemies at the moment
		{
			pendingDamage += baton._Value;
			playerBasicHit = playerBasicHit || baton._Id == ObjectId::PLAYER_PROJECTILE_BASIC;
			playerSpecialHit = playerSpecialHit || baton._Id == ObjectId::PLAYER_SPECIAL_ATTACK;
		}
	}

	/* Notify observers of the type of damage experienced */
	if (playerSpecialHit)
		observerController.NotifyObservers(GameObjectEvent::ENEMY_SPECIAL_DAMAGE);
	if (playerBasicHit)
		observerController.NotifyObservers(GameObjectEvent::ENEMY_BASIC_DAMAGE);

	return pendingDamage;
}

/*
	Description:
		Takes a damage value and applies it to an Enemies health property.
		Virtual to allow for different enemy types to respond to damage is differing ways.

	Arguments:
		pendingDamage - Amount of damage about to be applied to the enemy
		transform - Unused in base implementation. Typically damage elicits a movement based response (As seen in derived overrides)
		properties - The keeper of the Enemy's current health. Health is updated according to the pendingDamage value
*/
void EnemyNormalState::ApplyPendingDamage(const int& pendingDamage, TransformPt& transform, PController& properties)
{
	const int currentHealth = properties.GetPropertyValue(Property::HEALTH);
	if (pendingDamage > 0)
	{
		properties.SetPropertyValue(Property::HEALTH, currentHealth - pendingDamage);
	}
}