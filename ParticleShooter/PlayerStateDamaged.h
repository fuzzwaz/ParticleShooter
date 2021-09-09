#pragma once

#include "PlayerStateNormal.h"

class PlayerStateDamaged : public PlayerStateNormal
{
public:
	void OnStart(PropertyController& propController, const std::shared_ptr<Transform>& transform, const ObserverController& observerController) override;
	PlayerStateType Update(std::shared_ptr<Transform> transform, PropertyController& propController, const InputState& inputState, const ObserverController& observerController) override;

	PlayerStateType GetStateType() const override { return PlayerStateType::DAMAGED; }

protected:
	void TakeDamage(PropertyController& propController, const int& damage) override {} //Impervious to damage while in this state

private:
	/* Time in which Player is "damaged". While damaged, Player is impervious to other attacks */
	const float _damagedTime = 0.15;
	Timer _damagedTimer;
};