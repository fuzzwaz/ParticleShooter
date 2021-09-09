#include "GameObjectRegistryLocator.h"
#include "GameObjectRegistryService.h"
#include "ProjectileShooter.h"

ProjectileShooter::ProjectileShooter(std::shared_ptr<const Projectile> projectileType, const bool& shouldDelay) : _shouldDelay(shouldDelay), _projectilePrototype(projectileType) {}

ProjectileShooter::ProjectileShooter(const ProjectileShooter& source) : _shouldDelay(source._shouldDelay), _projectilePrototype(source._projectilePrototype) {}

/*
	Description:
		Ensures that the shooter has been delayed long enough. If so,
		Spawns a copy of _projectilePrototype and "Fires" it (applies velocity)
		
	Arguments:
		position - The position to spawn the Projectile at
		direction - The direction to fire the Projectile at
*/
void ProjectileShooter::Shoot(const Vector2& position, const Vector2& direction)
{
	const bool enoughTimeHasPassed = (_delayTimer.GetActive() && _delayTimer.GetSeconds() > _delay) || !_shouldDelay;
	if (!_delayTimer.GetActive() || enoughTimeHasPassed)
	{
		std::shared_ptr<Projectile> projectile = _projectilePrototype->Clone(position);
		GameObjectRegistryLocator::GetCreatorService()->AddGameObjectToScene(projectile);

		projectile->Fire(direction);
		_delayTimer.Start();
	}
}