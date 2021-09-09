#include "ErrorHandler.h"
#include "SoundManager.h"
#include "SoundAssetResources.h"

#include <SDL_mixer.h>

/*
	Description:
		Sets and confirms that the SDL_Mixer audio system is working.
		Required before constructing/initializing any sound objects
*/
SoundManager::SoundManager()
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		ErrorHandler::Assert(true, "Sound Manager failed to initialized");
	}

	CreateSoundEffectMappings();
	SetChannelVolume(SoundChannel::PLAYER_SOUND_EFFECT, 0.25);
}

SoundManager::~SoundManager()
{
	_soundEffectEventMap.clear();
}

void SoundManager::PlaySound(const std::shared_ptr<const Sound>& sound) const
{
	SetChannelVolume(sound->_ChannelType, sound->_Volume);

	if (sound->_ChannelType == SoundChannel::BACKGROUND_MUSIC)
	{
		Mix_PlayMusic(sound->_Song, sound->_Loops);
	}
	else
	{
		Mix_PlayChannel(static_cast<int>(sound->_ChannelType), sound->_Fx, sound->_Loops);
	}
	
}

void SoundManager::StopChannel(const SoundChannel& channel) const
{
	Mix_Pause(static_cast<int>(channel));
}

/*
	Description:
		Sets the volume on a specific audio channel

	Arguments:
		channel - The channel for which the volume is being adjusted
		volume - 0 means no volume. 1 means full volume.
*/
void SoundManager::SetChannelVolume(const SoundChannel& channel, float volume /*0 to 1*/) const
{
	if (volume > 1)
		volume = 1;
	else if (volume < 0)
		volume = 0;
	
	if (channel == SoundChannel::BACKGROUND_MUSIC)
	{
		Mix_VolumeMusic(MIX_MAX_VOLUME * volume);
	}
	else
	{
		Mix_Volume(static_cast<int>(channel), MIX_MAX_VOLUME * volume);
	}
}

/*
	Description:
		Plays the appropriate sound according the the given Event

	Arguments:
		eventType - The Event currently being dealt. Used as a key for the "_soundEffectEventMap"
		begin - True turns on the channel associated with the given event. False turns it off
*/
void SoundManager::Notify(const GameObjectEvent& eventType, const bool begin)
{
	//The Special Attack event is the only one which needs to be turned on and then off at unpredictable times
	if (eventType == GameObjectEvent::SPECIAL_ATTACK && _soundEffectEventMap.find(GameObjectEvent::SPECIAL_ATTACK) != _soundEffectEventMap.end())
	{
		const auto& laserSound = _soundEffectEventMap.at(GameObjectEvent::SPECIAL_ATTACK);

		if (begin)
			PlaySound(laserSound);
		else
			StopChannel(laserSound->_ChannelType);
	}
	else if (begin)
		Notify(eventType);
}

/*
	Description:
		Plays the appropriate sound according the the given Event

	Arguments:
		eventType - The Event currently being dealt. Used as a key for the "_soundEffectEventMap"
*/
void SoundManager::Notify(const GameObjectEvent& eventType)
{
	const auto foundMatchingSound = _soundEffectEventMap.find(eventType);

	if (foundMatchingSound != _soundEffectEventMap.end())
	{
		PlaySound(foundMatchingSound->second);

		if (eventType == GameObjectEvent::GAME_OVER || eventType == GameObjectEvent::VICTORY)
			StopPlayingAllSoundEffects();
	}
}

/*
	Description:
		Initializes all of the sound effects and pairs them with their corresponding
		GameObjectEvents within the "_soundEffectEventMap." 
*/
void SoundManager::CreateSoundEffectMappings()
{
	/* Player Specific */
	_soundEffectEventMap.insert({ GameObjectEvent::BASIC_ATTACK, std::make_shared<Sound>(Resources::Sounds::PLAYER_PROJECTILE_BASIC) });
	_soundEffectEventMap.insert({ GameObjectEvent::BOUNCE_ATTACK, std::make_shared<Sound>(Resources::Sounds::PLAYER_PROJECTILE_BOUNCE) });
	_soundEffectEventMap.insert({ GameObjectEvent::PLAYER_DAMAGE, std::make_shared<Sound>(Resources::Sounds::PLAYER_DAMAGE) });
	_soundEffectEventMap.insert({ GameObjectEvent::PLAYER_HEALTH_UP, std::make_shared<Sound>(Resources::Sounds::HEALTH_UP) });
	_soundEffectEventMap.insert({ GameObjectEvent::SPECIAL_ATTACK, std::make_shared<Sound>(Resources::Sounds::LASER_ACTIVE) });
	_soundEffectEventMap.insert({ GameObjectEvent::POWERED_UP, std::make_shared<Sound>(Resources::Sounds::POWER_UP) });
	_soundEffectEventMap.insert({ GameObjectEvent::POWERED_DOWN, std::make_shared<Sound>(Resources::Sounds::POWER_DOWN) });

	/* Enemy Specific */
	_soundEffectEventMap.insert({ GameObjectEvent::ENEMY_DEATH, std::make_shared<Sound>(Resources::Sounds::ENEMY_DEATH) });
	_soundEffectEventMap.insert({ GameObjectEvent::ENEMY_BASIC_DAMAGE, std::make_shared<Sound>(Resources::Sounds::ENEMY_DAMAGE_BASIC) });
	_soundEffectEventMap.insert({ GameObjectEvent::ENEMY_SPECIAL_DAMAGE, std::make_shared<Sound>(Resources::Sounds::ENEMY_DAMAGE_SPECIAL) });
	_soundEffectEventMap.insert({ GameObjectEvent::ENEMY_BASIC_ATTACK, std::make_shared<Sound>(Resources::Sounds::ENEMY_PROJECTILE_BASIC) });
	_soundEffectEventMap.insert({ GameObjectEvent::ENEMY_SPECIAL_ATTACK, std::make_shared<Sound>(Resources::Sounds::ENEMY_PROJECTILE_HEX) });
	_soundEffectEventMap.insert({ GameObjectEvent::ENEMY_TARGET_LOCKED, std::make_shared<Sound>(Resources::Sounds::ENEMY_TARGET_LOCKED) });

	/* Neutral */
	_soundEffectEventMap.insert({ GameObjectEvent::ROUND_START, std::make_shared<Sound>(Resources::Sounds::ROUND_START) });
	_soundEffectEventMap.insert({ GameObjectEvent::WAVE_START, std::make_shared<Sound>(Resources::Sounds::WAVE_START) });
	_soundEffectEventMap.insert({ GameObjectEvent::FINAL_WAVE_START, std::make_shared<Sound>(Resources::Sounds::WAVE_START) });
	_soundEffectEventMap.insert({ GameObjectEvent::VICTORY, std::make_shared<Sound>(Resources::Sounds::VICTORY) });
	_soundEffectEventMap.insert({ GameObjectEvent::GAME_OVER, std::make_shared<Sound>(Resources::Sounds::GAME_OVER) });
	_soundEffectEventMap.insert({ GameObjectEvent::LEVEL_START, std::make_shared<Sound>(Resources::Sounds::BACKGROUND_MUSIC) });
}

void SoundManager::StopPlayingAllSoundEffects() const
{
	StopChannel(SoundChannel::PLAYER_SOUND_EFFECT);
	StopChannel(SoundChannel::NEUTRAL_SOUND_EFFECT);
	StopChannel(SoundChannel::INTERFACE_SOUND_EFFECT);
	StopChannel(SoundChannel::ENEMY_SOUND_EFFECT);
	StopChannel(SoundChannel::ENEMY_SOUND_EFFECT2);
	StopChannel(SoundChannel::ENEMY_SOUND_EFFECT3);
}