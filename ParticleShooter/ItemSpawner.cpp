#include "ColliderResources.h"
#include "GameObjectRegistryLocator.h"
#include "GameObjectRegistryService.h"
#include "GraphicAssetResources.h"
#include "ItemSpawner.h"
#include "Rectangle.h"

#include <memory>

/*
	Description:
		Creates prototypes for the possible item drops and adds them to the _itemList along with their corresponding spawn rates.
*/
ItemSpawner::ItemSpawner()
{
	static const Item bouncePrototype(Rectangle(0, 0, 96, 96), Resources::Graphics::ITEM_BOUNCE, Resources::Colliders::ITEM_BOUNCE(), GameObjectEvent::POWERED_UP);
	static const Item healthPrototype(Rectangle(0, 0, 64, 64), Resources::Graphics::ITEM_HEALTH, Resources::Colliders::ITEM_HEALTH(), GameObjectEvent::PLAYER_HEALTH_UP);

	_itemList.emplace_back(bouncePrototype, _itemBounceSpawnRate);
	_itemList.emplace_back(healthPrototype, _itemHealthSpawnRate);
}

ItemSpawner& ItemSpawner::GetInstance()
{
	static ItemSpawner spawner;
	return spawner;
}

/*
	Description:
		Takes a random number out of 100 and compares it to the spawn rates of each Item in _itemList.
		If the number is in range of one of the spawn rates, that Item is spawned at the given position.
		
		NOTE: Is not guaranteed to create a new Item. Depends on random chance.

	Arguments:
		position: The location in world coordinates where are randomly selected Item could spawn.

		
*/
void ItemSpawner::SpawnItem(const Vector2& position)
{
	const int randomNumber = rand() % 100;

	std::shared_ptr<GameObject> item = nullptr;

	int upperBound = 0, bottomBound = 0;
	for(int i = 0; i < _itemList.size(); i++)
	{
		bottomBound = upperBound;
		upperBound += _itemList.at(i).second;

		if (bottomBound < randomNumber && randomNumber <= upperBound)
		{
			item = _itemList.at(i).first.Clone(position);
			break;
		}
	}

	if (item != nullptr)
	{
		GameObjectRegistryService* registry = GameObjectRegistryLocator::GetCreatorService();
		registry->AddGameObjectToScene(item);
	}
}