#include "AnimatedSingleTextureGraphicsController.h"
#include "ColliderResources.h"
#include "GameObjectProperties.h"
#include "GraphicAssetResources.h"
#include "Player.h"
#include "PlayerState.h"
#include "RigidBody.h"


Player::Player(const Vector2& startingPosition)
{
    SetUpDefaultPlayerProperties();
    SetUpGraphicsController();
    SetUpTransform(startingPosition);

    _playerState = &_playerStateNormal;
}


PlayerInfo Player::Update(const InputState& inputState)
{
    if (_updatesPaused)
        return _info;

    //Updates: Player's movement, attacks, and player properties based on current input and collision information
    PlayerStateType nextState = _playerState->Update(_transform, _propertyController, inputState, _observerController);

    if (nextState != _playerState->GetStateType())
        SwitchPlayerState(nextState);

    //Updates: Player Animation and current Texture
    _graphicsController->Update();

    //Return a small struct of relevant data that other objects can response to
    _info._CurrentPosition = _transform->GetOrigin();
    _info._CurrentPlayerState = _playerState->GetStateType();
    _info._CurrentHealth = _propertyController.GetPropertyValue(Property::HEALTH);
    _info._CurrentSpecial = _propertyController.GetPropertyValue(Property::SPECIAL_POWER);

    return _info;
}

#pragma region Private Methods

void Player::SwitchPlayerState(const PlayerStateType& state)
{
    _playerState->OnEnd(_transform);

    switch (state)
    {
    case PlayerStateType::NORMAL:
        _playerState = &_playerStateNormal;
        break;
    case PlayerStateType::BOUNCE:
        _playerState = &_playerStateBounce;
        break;
    case PlayerStateType::DAMAGED:
        _playerState = &_playerStateDamaged;
        break;
    case PlayerStateType::LASER:
        _playerState = &_playerStateLaser;
        break;
    default:
        _playerState = &_playerStateNormal;
    }
    
    _playerState->OnStart(_propertyController, _transform, _observerController);
}

void Player::SetUpDefaultPlayerProperties()
{
    _propertyController.SetPropertyValue(Property::IS_MOVING, false);
    _propertyController.SetPropertyValue(Property::IS_DAMAGED, false);
    _propertyController.SetPropertyValue(Property::POWER_ACTIVE, false);
    _propertyController.SetPropertyValue(Property::HEALTH, _startingHealth);
    _propertyController.SetPropertyValue(Property::SPECIAL_POWER, _startingSpecialPower);
}

void Player::SetUpTransform(const Vector2& startingPosition)
{
    _transform = std::make_shared<Transform>();
    _transform->SetSize(_width, _height);
    _transform->SetOrigin(startingPosition);
    _transform->_RigidBody.SetElasticityCoefficient(_playerElasticity);
    _transform->_Collider = Resources::Colliders::PLAYER();
    _transform->_Collider.SetAssociatedRigidBody(&_transform->_RigidBody);
}

void Player::SetUpGraphicsController()
{
    _graphicsController = std::make_shared<AnimatedSingleTextureGraphicsController>(&_propertyController);

    const std::shared_ptr<Animation> playerIdleAnimation = std::make_shared<Animation>(Resources::Graphics::PLAYER_IDLE);
    const std::shared_ptr<Animation> playedDamagedAnimation = std::make_shared<Animation>(Resources::Graphics::PLAYER_DAMAGED);

    playerIdleAnimation->AddTransition(Property::IS_DAMAGED, true, playedDamagedAnimation);
    playedDamagedAnimation->AddTransition(Transition(playerIdleAnimation));

    _graphicsController->SetCurrentAnimation(playerIdleAnimation);
}

#pragma endregion