//
//  SoundAssetResources.h
//  Particle Shooter
//
//  Created by Ramy Fawaz in 2021
//  Copyright (c) 2021 Ramy Fawaz. All rights reserved.
//

#pragma once

#include "SoundAssetInfo.h"

//A master list of all of the Sound Assets used in the game
namespace Resources
{
	namespace Sounds
	{
		/* Player Specific */
		const SoundAssetInfo POWER_DOWN("Assets/Sound/Effects/PowerDown.wav", SoundChannel::INTERFACE_SOUND_EFFECT, 0.1, 0);
		const SoundAssetInfo POWER_UP("Assets/Sound/Effects/PowerUp.wav", SoundChannel::NEUTRAL_SOUND_EFFECT, 0.1, 0);
		const SoundAssetInfo HEALTH_UP("Assets/Sound/Effects/HealthUp.wav", SoundChannel::NEUTRAL_SOUND_EFFECT, 0.2, 0);
		const SoundAssetInfo LASER_ACTIVE("Assets/Sound/Effects/Player_Laser_Sound.wav", SoundChannel::PLAYER_SOUND_EFFECT, 1, -1);
		const SoundAssetInfo PLAYER_PROJECTILE_BASIC("Assets/Sound/Effects/Player_Basic_Shot_Sound.wav", SoundChannel::PLAYER_SOUND_EFFECT, 0.25, 0);
		const SoundAssetInfo PLAYER_PROJECTILE_BOUNCE("Assets/Sound/Effects/Player_Bounce_Shot_Sound.wav", SoundChannel::PLAYER_SOUND_EFFECT, 0.25, 0);
		const SoundAssetInfo PLAYER_DAMAGE("Assets/Sound/Effects/Player_Damage.wav", SoundChannel::PLAYER_SOUND_EFFECT, 0.45, 0);

		/* Enemy Specific */
		const SoundAssetInfo ENEMY_DEATH("Assets/Sound/Effects/Basic_Enemy_Death.wav", SoundChannel::ENEMY_SOUND_EFFECT, 0.17, 0);
		const SoundAssetInfo ENEMY_DAMAGE_BASIC("Assets/Sound/Effects/Basic_Hit.wav", SoundChannel::ENEMY_SOUND_EFFECT, 0.12, 0);
		const SoundAssetInfo ENEMY_DAMAGE_SPECIAL("Assets/Sound/Effects/Special_Hit.wav", SoundChannel::ENEMY_SOUND_EFFECT, 0.4, 0);
		const SoundAssetInfo ENEMY_PROJECTILE_BASIC("Assets/Sound/Effects/Basic_Enemy_Hit.wav", SoundChannel::ENEMY_SOUND_EFFECT, 0.1, 0);
		const SoundAssetInfo ENEMY_PROJECTILE_HEX("Assets/Sound/Effects/Enemy_Hex_Shot.wav", SoundChannel::ENEMY_SOUND_EFFECT2, 0.3, 0);
		const SoundAssetInfo ENEMY_TARGET_LOCKED("Assets/Sound/Effects/Target_Locked.mp3", SoundChannel::ENEMY_SOUND_EFFECT3, 0.3, 1);

		/* Neutral */
		const SoundAssetInfo ROUND_START("Assets/Sound/Effects/RoundStart.wav", SoundChannel::INTERFACE_SOUND_EFFECT, 0.3, 0);
		const SoundAssetInfo WAVE_START("Assets/Sound/Effects/WaveStart.wav", SoundChannel::INTERFACE_SOUND_EFFECT, 1, 0);
		const SoundAssetInfo VICTORY("Assets/Sound/Music/Victory.mp3", SoundChannel::BACKGROUND_MUSIC, 0.65, -1);
		const SoundAssetInfo GAME_OVER("Assets/Sound/Music/GameOver.mp3", SoundChannel::BACKGROUND_MUSIC, 0.9, -1);
		const SoundAssetInfo BACKGROUND_MUSIC("Assets/Sound/Music/The_Speed_Consumes_Me.mp3", SoundChannel::BACKGROUND_MUSIC, 0.5, -1);
	}
}