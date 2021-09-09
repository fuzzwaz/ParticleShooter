#include "Cursor.h"
#include "HealthBar.h"
#include "SpecialBar.h"
#include "UserInterfaceManager.h"
#include "GameObjectRegistryLocator.h"
#include "GameObjectRegistryService.h"
#include "GraphicAssetResources.h"
#include "GraphicObject.h"

using std::shared_ptr;

UserInterfaceManager::UserInterfaceManager() :
_waveTitlePosition(Vector2(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2) - 300)), _centerScreenPosition(Vector2(SCREEN_WIDTH / 2, (SCREEN_HEIGHT / 2)))
{
	//Create Player specific elements and add them to the scene
	static const auto cursor = std::make_shared<Cursor>();
	GameObjectRegistryLocator::GetCreatorService()->AddGameObjectToScene(cursor);

	static const auto hBar = std::make_shared<HealthBar>(Vector2(15, SCREEN_HEIGHT - 150));
	GameObjectRegistryLocator::GetCreatorService()->AddGameObjectToScene(hBar);

	static const auto sBar = std::make_shared<SpecialBar>(Vector2(90, SCREEN_HEIGHT - 132));
	GameObjectRegistryLocator::GetCreatorService()->AddGameObjectToScene(sBar);

	//Initialize Title Screens and connect them to their appropriate GameObjectEvent in the event Map
	static std::shared_ptr<GraphicObject> _GameOverTitlePrototype = std::make_shared<GraphicObject>(Resources::Graphics::GAME_OVER_SCREEN);
	static std::shared_ptr<GraphicObject> _VictoryTitlePrototype = std::make_shared<GraphicObject>(Resources::Graphics::VICTORY_SCREEN);
	static std::shared_ptr<GraphicObject> _WaveTitlePrototype = std::make_shared<GraphicObject>(Resources::Graphics::WAVE_START, 2.75);
	static std::shared_ptr<GraphicObject> _FinalWaveTitlePrototype = std::make_shared<GraphicObject>(Resources::Graphics::FINAL_WAVE_START, 2.75);

	_eventMap.insert({ GameObjectEvent::WAVE_START, {_WaveTitlePrototype, _waveTitlePosition} });
	_eventMap.insert({ GameObjectEvent::FINAL_WAVE_START, {_FinalWaveTitlePrototype, _waveTitlePosition} });
	_eventMap.insert({ GameObjectEvent::VICTORY, {_VictoryTitlePrototype, _centerScreenPosition} });
	_eventMap.insert({ GameObjectEvent::GAME_OVER, {_GameOverTitlePrototype, _centerScreenPosition} });
}

void UserInterfaceManager::Notify(const GameObjectEvent& eventType)
{
	if (_eventMap.find(eventType) != _eventMap.end())
	{
		const auto uiElementPrototype = _eventMap.at(eventType);
		const auto uiElement = uiElementPrototype.first->Clone(uiElementPrototype.second);
		GameObjectRegistryLocator::GetCreatorService()->AddGameObjectToScene(uiElement);
	}
}