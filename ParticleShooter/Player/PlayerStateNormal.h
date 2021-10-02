//
//  PlayerStateNormal.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "PlayerState.h"
#include "ProjectileShooter.h"
#include "Timer.h"

#include <memory>

enum class ObjectId;
struct CollisionResponseInfo;

//The "Home" state for Player Movement and Attacks. Other states are temporary and will always revert back to this PlayerStateNormal at some point. 
class PlayerStateNormal : public PlayerState
{
public:
    PlayerStateNormal();

	void OnStart(PropertyController& propController, const std::shared_ptr<Transform>& transform, const ObserverController& observerController) override;
    PlayerStateType Update(std::shared_ptr<Transform> transform, PropertyController& propController, const InputState& inputState, const ObserverController& observerController) override;

	PlayerStateType GetStateType() const override { return PlayerStateType::NORMAL; }

protected:
    void UpdateMovement(const InputState& inputState, const std::shared_ptr<Transform>& transform) const;
    void UpdateAttacks(PropertyController& propController, const InputState& inputState, const std::shared_ptr<const Transform>& transform);

    CollisionResponseInfo ResolveCollisions(std::shared_ptr<Transform>& transform, PropertyController& propController, const ObserverController& observerController);

    void IncreaseHealth(PropertyController& propController, const int& health) const;
    virtual void TakeDamage(PropertyController& propController, const int& damage);

    bool _tookDamage = false;
    const float _movementSpeedNormal = 800;
    std::unique_ptr<ProjectileShooter> _projectileShooter = nullptr;

private:
    void RefillSpecialAbilityBar(PropertyController& propController);

    /* Time in which the Player's special ability power refills */
    const float _specialAbilityRefillTime = 1.75;
    Timer _specialAbilityRefillTimer;

    const int _maxHealth = 100;
    const int _maxSpecialAbilityPower = 100;
    const int _specialAbilityRefillAmount = 5;
    const float _movementAcclerationMagnitude = 6000;
    const float _projectileSpawnDistance = 50; //Distance form the Player in which to spawn Projectiles
};

bool CollidingWithPickUp(const CollisionResponseInfo& collisionInfo, const ObjectId& pickUpId);