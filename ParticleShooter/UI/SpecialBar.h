//
//  SpecialBar.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "AnimatedSingleTextureGraphicsController.h"
#include "GraphicAssetInfo.h"
#include "GraphicObject.h"
#include "PlayerInfo.h"

//Class representing the Player's Special Ability bar
class SpecialBar : public GraphicObject
{
public:
	SpecialBar(const Vector2& position) : GraphicObject(position)
	{
		const int fullSpecialPercentage = 100;
		_graphicsController = std::make_shared<AnimatedSingleTextureGraphicsController>(&_propertyController, Resources::Graphics::SPECIAL_BAR);
		_graphicsController->SetAnimationFramePercent(fullSpecialPercentage);
		SetIsActive(true);
	}

	void Update(const PlayerInfo& playerInfo, const Vector2& cameraPosition, const InputState& input) override
	{
		GraphicObject::Update(playerInfo, cameraPosition, input);
		_graphicsController->SetAnimationFramePercent(playerInfo._CurrentSpecial); //Updates how full the Special Ability bar looks 
	}
};
