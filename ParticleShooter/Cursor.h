//
//  Cursor.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "GraphicAssetResources.h"
#include "GraphicObject.h"
#include "InputState.h"
#include "Transform.h"

//Object representing the Player's cursor on the screen
class Cursor: public GraphicObject
{
public:
	Cursor() : GraphicObject(Resources::Graphics::CURSOR_ICON) { SetIsActive(true); }

	//Always update the Cursor's location to match the mouse position
	void Update(const PlayerInfo& playerInfo, const Vector2& cameraPosition, const InputState& input) override { _transform->SetOrigin(input._CursorPosition); }
};