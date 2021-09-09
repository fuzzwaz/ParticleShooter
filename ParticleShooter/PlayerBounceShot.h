//
//  PlayerBounceShot.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "Projectile.h"

//Nearly identical to the default except can bounce multiple times before being destroyed
class PlayerBounceShot : public Projectile
{
public:
	PlayerBounceShot();

	void Update(const PlayerInfo& playerInfo, const Vector2& cameraPosition, const InputState& input) override;
	std::shared_ptr<Projectile> Clone(const Vector2& position) const override;

private:
	const int _totalBounces = 3; //The number of bounces that the projectile makes before destroying itself
};
