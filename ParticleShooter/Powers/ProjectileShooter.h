//
//  ProjectileShooter.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "Projectile.h"

#include <memory>

//Spawns and fires Projectiles at a specified rate
class ProjectileShooter
{
public:
	ProjectileShooter(std::shared_ptr<const Projectile> projectileType, const bool& shouldDelay = true);
	ProjectileShooter(const ProjectileShooter& source);

	void Shoot(const Vector2& position, const Vector2& direction);

private:
	std::shared_ptr<const Projectile> _projectilePrototype = nullptr; //The Projectile instance that will be copied and fired

	double _delay = 0.15; //How often the Shooter is allowed to fire (seconds)
	Timer _delayTimer;

	const bool _shouldDelay = true; //Whether or not the delay timer should be ignored
};