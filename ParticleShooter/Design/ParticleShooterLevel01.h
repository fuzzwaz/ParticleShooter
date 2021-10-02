//
//  ParticleShooterLevel01.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "Collider.h"
#include "ParticleShooterLevel.h"
#include "Rectangle.h"

#include <array>

/*
	The first level in the ParticleShooter demo. Responsible for loading loading in all of the assets
	related to the level. Including: Graphics, collision information, and design layout.
*/
class ParticleShooterLevel01 : public ParticleShooterLevel
{
public:
	ParticleShooterLevel01();

	void LoadLevel() override;

	/* Getters */
	Vector2 GetCameraStart() const override;
	Vector2 GetPlayerStart() const override;
	std::vector<ColliderInterface*> GetLevelColliders() override;
	std::shared_ptr<const Transform> GetTransform() const override { return _transform; };
	std::shared_ptr<const GraphicsController> GetGraphicsController() const override { return _graphicsController; };

private:
	void LoadWaves();
	void LoadBackgroundImages();
	void LoadColliders();

	std::array<Collider, 25> _colliders; //All of the colliders associate with this level
	Rectangle _bounds = Rectangle(0, 0, 3000, 2000);

	std::shared_ptr<MultiTextureGraphicsController> _graphicsController = nullptr;
	std::shared_ptr<Transform> _transform = nullptr;
};