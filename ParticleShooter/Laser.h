//
//  Laser.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "GraphicObject.h"

//A Special Ability for the Player which activates a immovable laser that causes damage to any Enemy it touches
class Laser : public GraphicObject
{
public:
	Laser();

	void Update(const PlayerInfo& playerInfo, const Vector2& cameraPosition, const InputState& input) override;

	std::shared_ptr<const GraphicsController> GetGraphicsController() const override { return _graphicsController; }
	inline ColliderInterface* GetCollider() override;

private:
	void ToggleLaser(const bool& activate); //Turns the laser on and off
	void ReactToEnemyHits() const; //Responds to Enemy hits with graphic and sound effects

	std::shared_ptr<GraphicObject> _enemyHitParticles = nullptr; //Particle effect for when hitting enemies with the laser

	bool _playerUsingLaser = false; //Whether the laser is on or off
	const int _playerOffset = 50; //How far away the laser is positioned from the player
};