//
//  Player.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "ObserverController.h"
#include "PlayerInfo.h"
#include "PlayerState.h"
#include "PlayerStateBounce.h"
#include "PlayerStateDamaged.h"
#include "PlayerStateLaser.h"
#include "PlayerStateNormal.h"
#include "Transform.h"

class ColliderInterface;
struct InputState;

class Player
{
public:
	Player(const Vector2& startingPosition = Vector2(0,0));
	void AddObserver(GameObjectObserver* observer) { _observerController.AddObserver(observer); }

	PlayerInfo Update(const InputState& inputState);
	void PauseUpdates() { _updatesPaused = true; };

	/* Getters */
	ColliderInterface* GetCollider() { return &_transform->_Collider; }
	std::shared_ptr<const Transform> GetTransform() const { return _transform; }
	std::shared_ptr<const GraphicsController> GetGraphicsController() const { return _graphicsController; }

private:
	void SetUpDefaultPlayerProperties();
	void SetUpTransform(const Vector2& startingPosition);
	void SetUpGraphicsController();
	void SwitchPlayerState(const PlayerStateType& state);

	/* Initial Player Properties */
	const int _width = 64, _height = 64;
	const int _startingHealth = 100;
	const int _startingSpecialPower = 100;
	const float _playerElasticity = 0.4;

	std::shared_ptr<Transform> _transform = nullptr;
	std::shared_ptr<AnimatedSingleTextureGraphicsController> _graphicsController = nullptr;
	PropertyController _propertyController;
	ObserverController _observerController;

	PlayerInfo _info;

	PlayerStateNormal _playerStateNormal;
	PlayerStateDamaged _playerStateDamaged;
	PlayerStateBounce _playerStateBounce;
	PlayerStateLaser _playerStateLaser;
	PlayerState* _playerState = &_playerStateNormal;

	bool _updatesPaused = false;
};
