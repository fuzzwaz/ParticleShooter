#include "ColliderResources.h"
#include "GraphicAssetResources.h"
#include "HexBeam.h"
#include "Transform.h"

/*
	Description:
		Creates a Projectile with unique graphics and collision details
*/
HexBeam::HexBeam() : Projectile(Resources::Graphics::ENEMY_ATTACK_HEX)
{
	_destructionParticlesPrototype.reset();
	_destructionParticlesPrototype = nullptr;

	_transform->SetSize(190, 14);
	_transform->_Collider = Resources::Colliders::ENEMY_ATTACK_HEX();
	_transform->_RigidBody.SetElasticityCoefficient(0);
	_transform->_RigidBody.SetInvertedMass(0.1);

	_speed = 300;
	_fireEvent = GameObjectEvent::ENEMY_SPECIAL_ATTACK;
}

std::shared_ptr<Projectile> HexBeam::Clone(const Vector2& position) const
{
	std::shared_ptr<Projectile> clone = std::make_shared<HexBeam>(*this);
	clone->SetPosition(position);
	clone->SetIsActive(true);

	return clone;
}

/*
	Description:
		Updates the transform and checks whether or not the Projectile is ready to be destroyed.
		Projectile no longer is destroyed by Player basic Attacks.
		Non destroying collisions illicit no responses. 

	Arguments:
		playerInfo - Information about the Player during this frame.
		cameraPosition - Information about the camera's position in World Space
		input - Information about the System's input during this frame
*/
void HexBeam::Update(const PlayerInfo& playerInfo, const Vector2& cameraPosition, const InputState& input)
{
	GraphicObject::Update(playerInfo, cameraPosition, input);

	//Search for a collision that is not with a basic Player attack
	const auto collisionInfo = _transform->_Collider.GetCollisionResponseInfo();
	const auto foundCollision = std::find_if(collisionInfo._Batons.cbegin(), collisionInfo._Batons.cend(), [](const ColliderBaton& baton) -> bool
	{
		if (baton._Type != ColliderType::PLAYERATTACK || baton._Id != ObjectId::PLAYER_PROJECTILE_BASIC)
			return true;
		return false;
	});

	if (foundCollision != collisionInfo._Batons.cend()) //If found, destroy the object
		Destroy();
	else
	{
		_transform->_Collider.ClearCollisionResponseInfo(); //Clear the collision response info instead of responding to it. Nothing should alter their velocity vector
		Vector2 displacement = _transform->_RigidBody.ApplyMovementForces();
		_transform->Move(displacement);
	}
}
