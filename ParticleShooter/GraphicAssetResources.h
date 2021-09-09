//
//  GraphicAssetResources.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "GraphicAssetInfo.h"

//A master list of all of the Graphic Assets used in the game
namespace Resources
{
	namespace Graphics
	{
		/* UI */
		const GraphicAssetInfo LOADING_SCREEN("Assets/UI/Loading_", 1);
		const GraphicAssetInfo GAME_OVER_SCREEN("Assets/UI/Game_Over_", 9, 1, 3);
		const GraphicAssetInfo VICTORY_SCREEN("Assets/UI/Victory_", 9, 1, 3);

		const GraphicAssetInfo CURSOR_ICON("Assets/UI/HitMarker_", 1);
		const GraphicAssetInfo HEALTH_BAR("Assets/UI/Health_", 14, 1, 1, RenderingLayer::UI, false, true, SDL_FLIP_NONE, Vector2(0, 0));
		const GraphicAssetInfo SPECIAL_BAR("Assets/UI/Special_", 20, 1, 1, RenderingLayer::UI, false, true, SDL_FLIP_NONE, Vector2(0, 0));
		const GraphicAssetInfo WAVE_START("Assets/UI/Wave_Normal_", 11, 1, 3);
		const GraphicAssetInfo FINAL_WAVE_START("Assets/UI/Final_Wave_", 9, 1, 3);

		/* RENDERING EFFECTS */
		const GraphicAssetInfo RED_DIRECTED_PARTICLES("Assets/Particles/Generated/Small_Directed_Boom_", 14, 1, 20, RenderingLayer::PARTICLES, false);
		const GraphicAssetInfo PURPLE_PIXEL_PARTICLES("Assets/Particles/Generated/Purple_Pixel_Boom_", 25, 2.5, 30, RenderingLayer::PARTICLES, false);
		const GraphicAssetInfo LASER_HIT_PARTICLES("Assets/Particles/Generated/Laser_Hit_", 30, 1.35, 20, RenderingLayer::PARTICLES, false);
		const GraphicAssetInfo WARNING_BEAM_EFFECT("Assets/Particles/Square_Warning_", 7, 1, 8, RenderingLayer::ENEMIES, true, false, SDL_FLIP_NONE, Vector2(-0.5, -1), Vector2(0.5, 1));
		const GraphicAssetInfo LARGE_TRIANGLE_BOOM("Assets/Particles/Explosion/White_Triiangle_", 10, 1.5, 10, RenderingLayer::ENEMIES, false);
		const GraphicAssetInfo SMALL_TRIANGLE_BOOM(LARGE_TRIANGLE_BOOM, "Assets/Particles/Explosion/Orange_Triiangle_", 8);

		/* ITEMS */
		const GraphicAssetInfo ITEM_BOUNCE("Assets/Pick_Ups/Bounce_Pick_Up_", 16, 1, 4, RenderingLayer::ITEMS);
		const GraphicAssetInfo ITEM_HEALTH("Assets/Pick_Ups/Health_", 9, 1, 2.5, RenderingLayer::ITEMS);

		/* PLAYER */
		const GraphicAssetInfo PLAYER_IDLE("Assets/Player/Player_Idle_", 7, 1, 2, RenderingLayer::PLAYER);
		const GraphicAssetInfo PLAYER_DAMAGED("Assets/Player/Player_Damaged_", 8, 1, 15, RenderingLayer::PLAYER, false);
		const GraphicAssetInfo PLAYER_ATTACK_BASIC("Assets/Particles/Projectiles/PlayerShotAnim_", 6, 1, 6, RenderingLayer::PLAYER);
		const GraphicAssetInfo PLAYER_ATTACK_BOUNCE("Assets/Particles/Projectiles/PlayerBounceShotAnim_", 6, 1, 6, RenderingLayer::PLAYER);
		const GraphicAssetInfo PLAYER_ATTACK_LASER("Assets/Particles/Projectiles/Player_Laser_", 14, 1, 9, RenderingLayer::PLAYER, true, false, SDL_FLIP_NONE, Vector2(-0.5, -1), Vector2( 0.5, 1));

		/* ENEMIES */
		const GraphicAssetInfo ENEMY_ATTACK_BASIC("Assets/Particles/Projectiles/Enemy_Basic_", 8, 1, 6, RenderingLayer::ENEMIES);
		const GraphicAssetInfo ENEMY_ATTACK_HEX("Assets/Particles/Projectiles/Hexagon_Beam_", 6, 1, 3, RenderingLayer::ENEMIES);

		const GraphicAssetInfo ENEMY_HEXAGON_NORMAL("Assets/Enemies/Hexagon_Idle_", 9, 1, 3, RenderingLayer::ENEMIES, true, true);
		const GraphicAssetInfo ENEMY_HEXAGON_SPAWNING("Assets/Enemies/Hexagon_Spawning_", 6, 1, 4, RenderingLayer::ENEMIES, false);
		const GraphicAssetInfo ENEMY_HEXAGON_WARNING("Assets/Enemies/Hexagon_Warning_", 10, 1, 3, RenderingLayer::ENEMIES);
		const GraphicAssetInfo ENEMY_HEXAGON_DAMAGED("Assets/Enemies/Hexagon_Damaged_", 8, 1, 20, RenderingLayer::ENEMIES, false);

		const GraphicAssetInfo ENEMY_RHOMBUS_NORMAL("Assets/Enemies/Rhombus_Normal_", 7, 1, 3, RenderingLayer::ENEMIES);
		const GraphicAssetInfo ENEMY_RHOMBUS_SPAWNING("Assets/Enemies/Rhombus_Spawn_", 7, 1, 4, RenderingLayer::ENEMIES, false);
		const GraphicAssetInfo ENEMY_RHOMBUS_WARNING("Assets/Enemies/Rhombus_Warning_", 10, 1, 3, RenderingLayer::ENEMIES);
		const GraphicAssetInfo ENEMY_RHOMBUS_DAMAGED("Assets/Enemies/Rhombus_Damage_", 9, 1, 20, RenderingLayer::ENEMIES, false);

		const GraphicAssetInfo ENEMY_SQUARE_NORMAL("Assets/Enemies/Square_Idle_", 9, 1, 2, RenderingLayer::ENEMIES);
		const GraphicAssetInfo ENEMY_SQUARE_SPAWNING("Assets/Enemies/Square_Spawn_", 7, 1, 4, RenderingLayer::ENEMIES, false);
		const GraphicAssetInfo ENEMY_SQUARE_WARNING("Assets/Enemies/Square_Warning_", 10, 1, 3, RenderingLayer::ENEMIES);
		const GraphicAssetInfo ENEMY_SQUARE_DAMAGED("Assets/Enemies/Square_Damaged_", 8, 1, 20, RenderingLayer::ENEMIES, false);

	}
}