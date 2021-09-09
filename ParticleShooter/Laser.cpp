#include "ColliderResources.h"
#include "GraphicAssetResources.h"
#include "InputState.h"
#include "Laser.h"
#include "PlayerInfo.h"
#include "Transform.h"

Laser::Laser() : GraphicObject(Resources::Graphics::PLAYER_ATTACK_LASER)
{
	SetIsActive(true);
	_graphicsController->SetIsActive(false);
	_transform->_Collider = Resources::Colliders::PLAYER_ATTACK_LASER();

	_enemyHitParticles = std::make_shared<GraphicObject>(Resources::Graphics::LASER_HIT_PARTICLES, 0.15);
}

/*
	Description:
		Checks "playerInfo" to determine if the laser should be activated/deactivated.
		Updates the laser's position and direction to match Player input.

	Arguments:
		playerInfo - Gives us the Player's position and current state (whether or not they are in a laser state)
		cameraPosition - Current position of the camera in world space
		InputState - Gives us the position of the cursor which dictates the laser's direction
*/
void Laser::Update(const PlayerInfo& playerInfo, const Vector2& cameraPosition, const InputState& input)
{
	_graphicsController->Update();

	if (!_playerUsingLaser && playerInfo._CurrentPlayerState == PlayerStateType::LASER)
		ToggleLaser(true);
	else if (_playerUsingLaser && playerInfo._CurrentPlayerState != PlayerStateType::LASER)
		ToggleLaser(false);

	//If the laser is hitting an enemy, certain effects need to be displayed and played
	ReactToEnemyHits();

	//Calculate the direction of the laser using the difference vector of the Cursor and the Player's position
	Vector2 forward = input._CursorPosition - playerInfo._CurrentPosition;
	forward.Normalize();
	_transform->SetForwardVector(forward);

	//Place the laser slightly offset from the Player's current position
	_transform->SetOrigin(playerInfo._CurrentPosition + (forward * _playerOffset));
	_transform->_Collider.ClearCollisionResponseInfo();
}

/*
	Description:
		Turns the Laser's visuals and collision detection on or off. Sends a notification to all observers of this change.

	Arguments:
		activate - Whether it should be activated (true) or deactivated (false)
*/
void Laser::ToggleLaser(const bool& activate)
{
	_playerUsingLaser = activate;
	_graphicsController->SetIsActive(activate);
	_transform->_Collider.SetIsActive(activate);
	_observerController.NotifyObservers(GameObjectEvent::SPECIAL_ATTACK, activate);
}

/*
	Description:
		Checks to see if the laser is colliding with any enemies.
		If it is, spawns special hit particles at that collision location.
		Also, notifies observers that an Enemy has been hit but a special attack
*/
void Laser::ReactToEnemyHits() const
{
	const auto colInfo = _transform->_Collider.GetCollisionResponseInfo();
	for (const Vector2& colPos : colInfo._AllCollisionPoints) //Could put this on a timer
	{
		SpawnGraphicObject(*_enemyHitParticles, colPos);
	}
}

ColliderInterface* Laser::GetCollider()
{
	return &_transform->_Collider;
}