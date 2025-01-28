#include "AudioEngine.h"


#include "Errors.h"

namespace Dengine {
	void SoundEffect::play(int loops /*loops = 0*/)
	{
		if (Mix_PlayChannel(-1, chunk, loops) == -1) {
			if(Mix_PlayChannel(1, chunk, loops) == -1)
				fatalError("Mix_PlayChannelError: " + std::string(Mix_GetError()));
		}
	}

	void Music::play(int loops /*loops = -1*/)
	{
		if (Mix_PlayMusic(music, loops) == -1) {
			fatalError("Mix_PlayMusic Error: " + std::string(Mix_GetError()));
		}
	}

	void Music::pause()
	{
		Mix_PauseMusic();
	}

	void Music::stop()
	{
		Mix_HaltMusic();
	}

	void Music::resume()
	{
		Mix_ResumeMusic();
	}

	AudioEngine::AudioEngine()
	{
	}

	AudioEngine::~AudioEngine()
	{
		destroy();
	}

	void AudioEngine::init()
	{
		/*if (isInitialized) {
			fatalError("Tried to initialize same Audio Engine Twice");
		}*/

		//Parameter takes a bitwise combo of initialization parametes corresponding to various audio file types
		//MIX_INIT_MOD, MIX_INIT_MP3, MIX_INIT_OGG, MIX_INIT_FAC, wav files supported by default
		if ( Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG | MIX_INIT_FLAC) == -1) {
			fatalError( "Mix_Init error: " + std::string(Mix_GetError()) );
		}

		if ( Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, AUDIO_F32SYS, 2, 1024) == -1) {
			fatalError("Mix_OpenAudio error: " + std::string(Mix_GetError()));
		}

		isInitialized = true;
	}

	void AudioEngine::destroy()
	{
		if (isInitialized) {
			isInitialized = false;
			

			for (auto& it : effectMap) {
				Mix_FreeChunk(it.second);
			}

			for (auto& it : musicMap) {
				Mix_FreeMusic(it.second);
			}
			effectMap.clear();
			musicMap.clear();
			Mix_CloseAudio();
			Mix_Quit();
		}
	}

	SoundEffect AudioEngine::loadSoundEffect(const std::string& filePath)
	{
		auto it = effectMap.find(filePath);
		SoundEffect effect;

		//not already present in map, add to map
		if (it == effectMap.end()) {
			Mix_Chunk* chunk = Mix_LoadWAV( filePath.c_str() );
			
			//error check
			if (chunk == nullptr) {
				fatalError("Mix_LoadWAV error: " + std::string(Mix_GetError()));
			}

			//add to map cache and set return value
			effectMap[filePath] = chunk;
			effect.chunk = chunk;
		}

		//already found in map
		else {
			effect.chunk = it->second;
		}

		return effect;
	}

	Music AudioEngine::loadMusic(const std::string& filePath)
	{
		auto it = musicMap.find(filePath);
		Music _music;

		//not already present in map, add to map
		if (it == musicMap.end()) {
			Mix_Music* mixMusic = Mix_LoadMUS(filePath.c_str());

			//error check
			if (mixMusic == nullptr) {
				fatalError("Mix_LoadMUS error: " + std::string(Mix_GetError()));
			}

			//add to map cache and set return value
			musicMap[filePath] = mixMusic;
			_music.music = mixMusic;
		}

		//already found in map
		else {
			_music.music = it->second;
		}

		return _music;
	}
	
}