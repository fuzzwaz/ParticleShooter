#include "GameObjectObserver.h"
#include "GraphicObject.h"
#include "PlayerBounceShot.h"
#include "GraphicAssetResources.h"
#include "GameObjectProperties.h"
#include "Transform.h"

/*
	Description:
		Inherits all of the default properties from Projectile but changes the graphics entirely.
		Implements one animation for when the shots still have "bounces" left in them.
		Transitions to another animation when the shot is on their final "bounce" and will be destroyed on collision
*/
PlayerBounceShot::PlayerBounceShot() : Projectile()
{
	_graphicsController.reset(new AnimatedSingleTextureGraphicsController(&_propertyController));
	const std::shared_ptr<Animation> preBounce = std::make_shared<Animation>(Resources::Graphics::PLAYER_ATTACK_BOUNCE);
	const std::shared_ptr<Animation> postBounce = std::make_shared<Animation>(Resources::Graphics::PLAYER_ATTACK_BASIC);
	preBounce->AddTransition(Property::BOUNCE_COUNT, 1, postBounce); //Transition to new animation when only once bounce left
	_graphicsController->SetCurrentAnimation(preBounce);

	_propertyController.SetPropertyValue(Property::BOUNCE_COUNT, _totalBounces);

	_fireEvent = GameObjectEvent::BOUNCE_ATTACK;
}

std::shared_ptr<Projectile> PlayerBounceShot::Clone(const Vector2& position) const
{
	std::shared_ptr<Projectile> clone = std::make_shared<PlayerBounceShot>(*this);
	clone->SetPosition(position);
	clone->SetIsActive(true);

	return clone;
}

/*
	Description:
		Updates the transform and checks whether or not the Projectile is ready to be destroyed.
		Instead of destroying on impact, lowers the _bounceCount. Destroys once _bounceCount gets to 0.

	Arguments:
		playerInfo - Information about the Player during this frame.
		cameraPosition - Information about the camera's position in World Space
		input - Information about the System's input during this frame
*/
void PlayerBounceShot::Update(const PlayerInfo& playerInfo, const Vector2& cameraPosition, const InputState& input)
{
	GraphicObject::Update(playerInfo, cameraPosition, input);

	const auto info = _transform->_Collider.GetCollisionResponseInfo();
	if (info._IsColliding)
	{
		const int newBounceCount = _propertyController.GetPropertyValue(Property::BOUNCE_COUNT) - 1;
		_propertyController.SetPropertyValue(Property::BOUNCE_COUNT, newBounceCount);
		if (newBounceCount == 1)
			_propertyController.SetPropertyValue(Property::IS_COLLIDING, true);
		if (newBounceCount <= 0)
		{
			Destroy();
			return;
		}
	}

	_transform->ResolveCollisions();
	Vector2 displacement = _transform->_RigidBody.ApplyMovementForces();
	_transform->Move(displacement);
}