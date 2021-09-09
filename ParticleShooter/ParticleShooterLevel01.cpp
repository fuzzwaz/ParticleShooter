#include "ColliderResources.h"
#include "ErrorHandler.h"
#include "ParticleShooterLevel01.h"
#include "Transform.h"

#include <nlohmann/json.hpp>
#include <string>
#include <fstream>

using json = nlohmann::json;

ParticleShooterLevel01::ParticleShooterLevel01()
{
	_transform.reset(new Transform(_bounds));
}

void ParticleShooterLevel01::LoadLevel()
{
	LoadWaves();
	LoadBackgroundImages();
	LoadColliders();
}

Vector2 ParticleShooterLevel01::GetCameraStart() const
{
	return Vector2(_bounds._Width / 4, _bounds._Height / 4);
}

Vector2 ParticleShooterLevel01::GetPlayerStart() const
{
	return Vector2((_bounds._Width / 2) + 200, (_bounds._Height / 2) + 50);
}


/*
	Description:
		Opens a json level layout map exported using the Tiled Level Editor. Each "layer" in the json
		containers information about the Enemies in each Round and Wave.

		Constructs Wave and Round objects in accordance to the file.
 */
void ParticleShooterLevel01::LoadWaves()
{
	const std::string filePath = "Assets/World/Level_1.json";
	std::ifstream fileStream(filePath, std::ifstream::in);
	ErrorHandler::Assert(fileStream.good(), "Level: " + filePath + " did not open successfully.");

	json jsonFile;
	fileStream >> jsonFile;
	auto layers = jsonFile["layers"];

	for (auto layer : layers)
	{
		auto properties = layer["properties"];

		int round = -1, wave = -1;
		for (auto pIt = properties.cbegin(); pIt < properties.cend(); pIt++)
		{
			auto property = (*pIt);
			if (property["name"] == "Round")
			{
				round = property["value"];
			}
			else if (property["name"] == "Wave")
			{
				wave = property["value"];
			}
		}

		if (round >= 0 && wave >= 0)
		{
			Wave* currentWave = GetWave(wave);
			Round* currentRound = currentWave->GetRound(round);

			for (auto object : layer["objects"])
			{
				auto objProperties = object["properties"];
				for (auto pIt = objProperties.cbegin(); pIt < objProperties.cend(); pIt++)
				{
					auto property = (*pIt);
					if (property["name"] == "EnemyType")
					{
						int type = property["value"];
						const Vector2 position(object["x"], object["y"]);
						currentRound->AddEnemyToSpawn(type, position);
					}
				}
			}
		}
	}

}

/*
	Description:
		Adds a variety of background PNGs representing the Level background to the graphics controller.
 */
void ParticleShooterLevel01::LoadBackgroundImages()
{
	_graphicsController.reset(new MultiTextureGraphicsController());
	_graphicsController->SetRenderingLayer(RenderingLayer::LEVEL);

	_graphicsController->AddTexture("Assets/World/Layer_04.png", 0, 0);
	_graphicsController->AddTexture("Assets/World/Layer_04.png", 1500, 0);
	_graphicsController->AddTexture("Assets/World/Layer_04.png", 0, 1000);
	_graphicsController->AddTexture("Assets/World/Layer_04.png", 1500, 1000);

	_graphicsController->AddTexture("Assets/World/Layer_03_01.png", 0 - 150, 0 - 175, 0.7);
	_graphicsController->AddTexture("Assets/World/Layer_03_02.png", 1500 - 150, 0 - 175, 0.7);
	_graphicsController->AddTexture("Assets/World/Layer_03_03.png", 0 - 150, 1000 - 175, 0.7);
	_graphicsController->AddTexture("Assets/World/Layer_03_04.png", 1500 - 150, 1000 - 175, 0.7);

	_graphicsController->AddTexture("Assets/World/Layer_02_01.png", 0 - 50, -100, 0.85);
	_graphicsController->AddTexture("Assets/World/Layer_02_02.png", 1500 - 50, -100, 0.85);
	_graphicsController->AddTexture("Assets/World/Layer_02_03.png", 0 - 50, 1000 - 100, 0.85);
	_graphicsController->AddTexture("Assets/World/Layer_02_04.png", 1500 - 50, 1000 - 100, 0.85);

	_graphicsController->AddTexture("Assets/World/Layer_01_01.png", 0, 0);
	_graphicsController->AddTexture("Assets/World/Layer_01_02.png", 1500, 0);
	_graphicsController->AddTexture("Assets/World/Layer_01_03.png", 0, 1000);
	_graphicsController->AddTexture("Assets/World/Layer_01_04.png", 1500, 1000);
}

/*
	Description:
		Opens a json collider map exported using the Tiled Level Editor. Each "object" in the json
		file containers information about an environmental collider in the level.

		Fills the _colliders array with this data.
 */
void ParticleShooterLevel01::LoadColliders()
{
	_transform->_RigidBody.SetInvertedMass(0);
	_transform->_RigidBody.SetElasticityCoefficient(0);

	std::string fileName = "Assets/World/Planet_01_Map.json";
	std::ifstream fileStream(fileName, std::ifstream::in);
	ErrorHandler::Assert(fileStream.good(), "Colliders from file: " + fileName + " did not open successfully.");
	json jsonFile;
	fileStream >> jsonFile;
	auto layers = jsonFile["layers"];
	auto layer = layers[1];
	auto objs = layer["objects"];
	auto it = objs.begin();
	int id = 0;
	while (it != objs.end())
	{
		auto object = *it;
		double xOrigin = object["x"];
		double yOrigin = object["y"];

		auto polygon = object["polygon"];
		auto pIt = polygon.begin();

		std::vector<Vector2> vertices;
		while (pIt != polygon.end())
		{
			auto pair = *pIt;

			double x = pair["x"];
			double y = pair["y"];

			vertices.push_back(Vector2(x + xOrigin, y + yOrigin));

			pIt++;
		}

		_colliders[id] = Resources::Colliders::ENVIRONMENT();
		_colliders[id].SetPosition(_transform->GetOrigin());
		_colliders[id]._Polygon.AddVertexPoint(vertices);
		_colliders[id].SetAssociatedRigidBody(&_transform->_RigidBody);

		it++;
		id++;
	}
}

/*
	Description:
		Returns interfaces to all of the environmental colliders associated with this level
 */
std::vector<ColliderInterface*> ParticleShooterLevel01::GetLevelColliders()
{
	std::vector<ColliderInterface*> _colliderInterfaces;

	for (Collider& collider : _colliders)
	{
		_colliderInterfaces.push_back(&collider);
	}

	return _colliderInterfaces;
}
