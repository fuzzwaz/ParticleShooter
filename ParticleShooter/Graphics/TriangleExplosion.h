//
//  TriangleExplosion.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "GraphicObject.h"

//An animated graphic where a barrage of randomly placed triangles rapidly explode on the screen
class TriangleExplosion : public GraphicObject
{
public:
	TriangleExplosion(const float& scale = 1);
	TriangleExplosion(const TriangleExplosion& source);

	std::shared_ptr<GraphicObject> Clone(const Vector2& position, const Vector2& direction) const override;
	void Update(const PlayerInfo& playerInfo, const Vector2& cameraPosition, const InputState& input) override;

private:
	/* Prototypes used to spawn the triangles when needed */
	std::shared_ptr<GraphicObject> _triangleWhiteLargePrototype = nullptr;
	std::shared_ptr<GraphicObject> _triangleRedSmallPrototype = nullptr;
	std::shared_ptr<GraphicObject> _trianglePurpleSmallPrototype = nullptr;
	std::shared_ptr<GraphicObject> _triangleOrangeSmallPrototype = nullptr;

	const int _randomOffsetMagnitude = 70; //Used to determine how far offset the small triangle spawn position can be
	const float _destructionTime = 0.75;
	const float _redSpawnTime = 0.1, _orangeSpawnTime = 0.15, _purpleSpawnTime = 0.2; //The times at which each small triangle spawns. Spread out for dramatic effect
	bool _spawnedWhite = false, _spawnedRed = false, _spawnedOrange = false, _spawnedPurple = false; //Whether or not each of the triangles have spawned
	Timer _spawnTimer;
};