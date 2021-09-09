#include "Enemy.h"
#include "EnemySpawningState.h"
#include "Transform.h"

std::shared_ptr<EnemyState> EnemySpawningState::Clone() const
{
	return std::make_shared<EnemySpawningState>();
}

/*
	Description:
		Disable Physics while spawning. Start the _spawnTimer which decides
		how long an Enemy stays in this State.

	Arguments:
		observerController - Unused in this derived implementation
		graphics - Activated to make sure the spawning animations are visible
		transform - Used to disable the collider
		properties - Unused in this derived implementation
*/
void EnemySpawningState::OnStart(ObserverController& observerController, TransformPt& transform, GControllerPt& graphics, PController& properties)
{
	transform->_Collider.SetIsActive(false);
	graphics->SetIsActive(true);

	_warningComplete = false;
	_spawnTimer.Start();
}

/*
	Description:
		Waits for the _spawnTimer to elapse _spawnTime while managing the warning animation.
		Once _spawnTime is elapsed, turn the Collider back on and transition to the Normal state.
		
	Arguments:
		playerInfo - Unused in this derived implementation
		observerController - Unused in this derived implementation
		transform - Used to enable the collider 
		properties - Controls the animations. Allows us to switch from the warning animation to the spawning animation to the normal animation
*/
EnemyStateType EnemySpawningState::Update(const PlayerInfo& playerInfo, ObserverController& observerController, TransformPt& transform, GControllerPt& graphics, PController& properties)
{
	if (_spawnTimer.GetSeconds() >= _spawnTime)
	{
		transform->_Collider.SetIsActive(true);
		return EnemyStateType::NORMAL;
	}
	else if (!_warningComplete && _spawnTimer.GetSeconds() >= _warningTime)
	{
		properties.SetPropertyValue(Property::OBJECT_STATE, static_cast<int>(NORMAL));
		_warningComplete = true;
	}

	return EnemyStateType::SPAWNING;
}