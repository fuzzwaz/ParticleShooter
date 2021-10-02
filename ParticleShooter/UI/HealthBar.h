//
//  HealthBar.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "GraphicAssetResources.h"
#include "GraphicObject.h"
#include "PlayerInfo.h"

//Class representing the Player's health bar
class HealthBar : public GraphicObject
{
public:
	HealthBar(const Vector2& position) : GraphicObject(position)
	{
		const int fullHealthPercentage = 100;
		_graphicsController = std::make_shared<AnimatedSingleTextureGraphicsController>(&_propertyController, Resources::Graphics::HEALTH_BAR);
		_graphicsController->SetAnimationFramePercent(fullHealthPercentage);
		SetIsActive(true);
	}

	void Update(const PlayerInfo& playerInfo, const Vector2& cameraPosition, const InputState& input) override
	{
		GraphicObject::Update(playerInfo, cameraPosition, input);
		_graphicsController->SetAnimationFramePercent(playerInfo._CurrentHealth); //Updates how full the health bar looks 
	}
};
