//
//  Projectile.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "GraphicObject.h"

#include <memory>

class ColliderInterface;

//The base for all projectile attacks for both the Player and Enemies
class Projectile : public GraphicObject
{
public:
	Projectile();
	Projectile(const Projectile& source);
	Projectile(const GraphicAssetInfo& media);
	virtual ~Projectile() override;

	void Fire(const Vector2& direction); //Applies an impulse velocity in the specified direction
	virtual std::shared_ptr<Projectile> Clone(const Vector2& position) const;

	virtual void Update(const PlayerInfo& playerInfo, const Vector2& cameraPosition, const InputState& input) override;
	inline ColliderInterface* GetCollider() override;

protected:
	void Destroy();


	float _speed = 1200; //Velocity magnitude
	GameObjectEvent _fireEvent = GameObjectEvent::BASIC_ATTACK; //Event that's applied when the Projectile is fired

	/* Projectiles have the option of emitting a Particle graphic effect upon collision */
	std::shared_ptr<GraphicObject> _destructionParticlesPrototype = nullptr;
	float _destructionParticleDistance = 5; //How fair away to spawn Particles 

private:
	const float _destructionTime = 10; //All Particles self destroy after this many seconds
};
