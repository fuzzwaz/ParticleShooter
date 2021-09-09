#include "Collider.h"
#include "ColliderResources.h"
#include "GraphicAssetResources.h"
#include "GraphicObject.h"
#include "Projectile.h"
#include "TimedDestructionController.h"
#include "Transform.h"

Projectile::Projectile() : Projectile(Resources::Graphics::PLAYER_ATTACK_BASIC) {}

Projectile::Projectile(const Projectile& source) : GraphicObject(source)
{
	_destructionParticlesPrototype = source._destructionParticlesPrototype;
	_transform->_Collider.SetAssociatedRigidBody(&_transform->_RigidBody);

	_speed = source._speed;
	_destructionParticleDistance = source._destructionParticleDistance;
	_fireEvent = source._fireEvent;
}

Projectile::Projectile(const GraphicAssetInfo& media) : GraphicObject(media)
{
	_destructionController->SetDestructionTime(_destructionTime);

	static const std::shared_ptr<GraphicObject> redParticles = std::make_shared<GraphicObject>(Resources::Graphics::RED_DIRECTED_PARTICLES, 1);
	_destructionParticlesPrototype = redParticles;

	//Transform Defaults for Projectiles
	const float elasticity = 1, invertedMass = 0.9, width = 32, height = 32;

	_transform->_RigidBody.SetElasticityCoefficient(elasticity);
	_transform->_RigidBody.SetInvertedMass(invertedMass);
	_transform->SetSize(width, height);
	_transform->_RigidBody.SetDecelerationSpeed(0); //Projectiles don't decelerate 
	_transform->_Collider = Resources::Colliders::PLAYER_ATTACK_BASIC();
}

Projectile::~Projectile()
{
	_destructionParticlesPrototype.reset();
}

std::shared_ptr<Projectile> Projectile::Clone(const Vector2& position) const
{
	std::shared_ptr<Projectile> clone = std::make_shared<Projectile>(*this);
	clone->SetPosition(position);
	clone->SetIsActive(true);

	return clone;
}

/*
	Description:
		Applies an impulse velocity to the Projectile in the specified direction.
		Notifies observers that the Projectile has been fired

	Arguments:
		direction - Will be normalized and used as the direction for the impulse vector
*/
void Projectile::Fire(const Vector2& direction)
{
	_transform->SetForwardVector(direction);
	_transform->_RigidBody.AddImpulseVelocity(direction.Normal() * _speed);
	_observerController.NotifyObservers(_fireEvent);
}

/*
	Description:
		Updates the transform and checks whether or not the Projectile is ready to be destroyed.
		Default behavior for Projectiles is to destroy them on collision with any object.

	Arguments:
		playerInfo - Information about the Player during this frame.
		cameraPosition - Information about the camera's position in World Space
		input - Information about the System's input during this frame
*/
void Projectile::Update(const PlayerInfo& playerInfo, const Vector2& cameraPosition, const InputState& input)
{
	GraphicObject::Update(playerInfo, cameraPosition, input);

	if (_transform->_Collider.GetCollisionResponseInfo()._IsColliding)
	{
		Destroy();
	}
	else
	{
		_transform->ResolveCollisions();
		const Vector2 displacement = _transform->_RigidBody.ApplyMovementForces();
		_transform->Move(displacement);
	}
}

ColliderInterface* Projectile::GetCollider()
{
	return &_transform->_Collider;
}

/*
	Description:
		Notifies observers that destruction is imminent. Sets an inactive no so more updates occur.
		If a _destructionParticlePrototype was set, spawns one at the place of impact.
*/
void Projectile::Destroy()
{
	if (_destructionParticlesPrototype != nullptr)
	{
		const Vector2 forward = _transform->_Collider.GetCollisionResponseInfo()._NudgeDirection * -1;
		const Vector2 position = _transform->GetOrigin() + (forward * _destructionParticleDistance);
		SpawnGraphicObject(*_destructionParticlesPrototype, position, forward);
	}

	_observerController.NotifyObservers(GameObjectEvent::DESTROYED);
	SetIsActive(false);
}