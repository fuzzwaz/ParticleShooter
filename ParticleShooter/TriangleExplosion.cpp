#include "TriangleExplosion.h"
#include "GameObjectRegistryService.h"
#include "GraphicAssetResources.h"
#include "Transform.h"
#include "TimedDestructionController.h"

TriangleExplosion::TriangleExplosion(const float& scale) : GraphicObject()
{
	GraphicAssetInfo largeTriangle = Resources::Graphics::LARGE_TRIANGLE_BOOM;
	largeTriangle._TextureScale *= scale;

	GraphicAssetInfo smallTriangle = Resources::Graphics::SMALL_TRIANGLE_BOOM;
	smallTriangle._TextureScale = scale;

	_triangleWhiteLargePrototype = std::make_shared<GraphicObject>(largeTriangle, _destructionTime);
	_triangleOrangeSmallPrototype = std::make_shared<GraphicObject>(smallTriangle, _destructionTime);
	_trianglePurpleSmallPrototype = std::make_shared<GraphicObject>(GraphicAssetInfo(smallTriangle, "Assets/Particles/Explosion/Purple_Triiangle_"), _destructionTime);
	_triangleRedSmallPrototype = std::make_shared<GraphicObject>(GraphicAssetInfo(smallTriangle, "Assets/Particles/Explosion/Red_Triiangle_"), _destructionTime);

	_destructionController->SetDestructionTime(_destructionTime);
	_graphicsController->SetIsActive(false);
	_graphicsController->SetRenderingLayer(RenderingLayer::ENEMIES);
}

TriangleExplosion::TriangleExplosion(const TriangleExplosion& source)
{
	_triangleWhiteLargePrototype = source._triangleWhiteLargePrototype;
	_triangleOrangeSmallPrototype = source._triangleOrangeSmallPrototype;
	_trianglePurpleSmallPrototype = source._trianglePurpleSmallPrototype;
	_triangleRedSmallPrototype = source._triangleRedSmallPrototype;
	*_graphicsController = *source._graphicsController;
	*_destructionController = *source._destructionController;
}

/*
	Description:
		Creates a new active copy  which shares the same animator graphics for each of the triangles prototypes.
		This avoids the re-initialization of the textures and animations associated with a graphic object.

	Arguments:
		position - The initial position for the cloned Triangle Explosion in world coordinates
				   The smaller triangles will be spawned with a random offset off of this position
*/
std::shared_ptr<GraphicObject> TriangleExplosion::Clone(const Vector2& position, const Vector2& direction) const
{
	std::shared_ptr<TriangleExplosion> clone = std::make_shared<TriangleExplosion>(*this);
	clone->SetPosition(position);
	clone->SetIsActive(true);

	clone->_spawnTimer.Start();

	return clone;
}

/*
	Description:
		Regularly checks the state of _spawnTimer to determine which triangle needs to be spawned.
		Gives each triangle an appropriate position to spawn at when the time comes. Once spawned, triangles (GraphicObjects)
		manage themselves (destroy themselves at after a certain amount of time)

	Arguments:
		playerInfo - Ignored. Information about the player's current state.
		cameraPosition - Ignored. The camera's position to help determine Camera space coordinates
		input - Ignored. The current input recieved by the InputManager
*/
void TriangleExplosion::Update(const PlayerInfo& playerInfo, const Vector2& cameraPosition, const InputState& input)
{
	GraphicObject::Update(playerInfo, cameraPosition, input);

	if (_spawnTimer.GetSeconds() >= _purpleSpawnTime && !_spawnedPurple)
	{
		_spawnedPurple = true;
		const Vector2 randomOffset(CommonHelpers::RandomOffset(_randomOffsetMagnitude));
		SpawnGraphicObject(*_trianglePurpleSmallPrototype, _transform->GetOrigin() + randomOffset);

	}
	else if (_spawnTimer.GetSeconds() >= _orangeSpawnTime && !_spawnedOrange)
	{
		_spawnedOrange = true;
		const Vector2 randomOffset(CommonHelpers::RandomOffset(_randomOffsetMagnitude));
		SpawnGraphicObject(*_triangleOrangeSmallPrototype, _transform->GetOrigin() + randomOffset);
	}
	else if (_spawnTimer.GetSeconds() >= _redSpawnTime && !_spawnedRed)
	{
		_spawnedRed = true;
		const Vector2 randomOffset(CommonHelpers::RandomOffset(_randomOffsetMagnitude));
		SpawnGraphicObject(*_triangleRedSmallPrototype, _transform->GetOrigin() + randomOffset);
	}

	if (!_spawnedWhite)
	{
		_spawnedWhite = true;
		SpawnGraphicObject(*_triangleWhiteLargePrototype, _transform->GetOrigin());
	}
}