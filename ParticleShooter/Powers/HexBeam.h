//
//  HexBeam.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "Projectile.h"

//A rectangular enemy projectile that is immune to collision response
class HexBeam : public Projectile
{
public:
	HexBeam();
	void Update(const PlayerInfo& playerInfo, const Vector2& cameraPosition, const InputState& input) override;
	std::shared_ptr<Projectile> Clone(const Vector2& position) const override;
};