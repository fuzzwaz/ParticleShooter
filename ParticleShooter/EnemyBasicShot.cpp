#include "ColliderResources.h"
#include "EnemyBasicShot.h"
#include "GraphicAssetResources.h"
#include "GraphicObject.h"
#include "Transform.h"

/*
	Description:
		Creates a Projectile with unique graphics and collision details
*/
EnemyBasicShot::EnemyBasicShot() : Projectile(Resources::Graphics::ENEMY_ATTACK_BASIC)
{
	_transform->SetSize(32, 48);
	_transform->_Collider = Resources::Colliders::ENEMY_ATTACK_BASIC();

	_destructionParticleDistance = 0;
	const std::shared_ptr<GraphicObject> purpleParticles = std::make_shared<GraphicObject>(Resources::Graphics::PURPLE_PIXEL_PARTICLES, 0.5);
	_destructionParticlesPrototype = purpleParticles;

	_fireEvent = GameObjectEvent::ENEMY_BASIC_ATTACK;
}