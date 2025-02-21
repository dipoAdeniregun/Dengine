#include "Timing.h"
#include <sdl/SDL.h>
#include <type_traits>

namespace Dengine {
	FPSLimiter::FPSLimiter(){}
	
	void FPSLimiter::init(float _maxFPS){
		setMaxFPS(_maxFPS);
	}

	void FPSLimiter::setMaxFPS(float _maxFPS) { maxFPS = _maxFPS; }
	
	void FPSLimiter::beginFrame(){
		startTicks = SDL_GetTicks();
	}

	float FPSLimiter::endFrame(){
		calculateFPS();

		float endTicks = SDL_GetTicks() - startTicks;

		//if faster than 60FPS, limit to 60FPS
		if (1000.0f / maxFPS > endTicks) {
			SDL_Delay((1000.0f / maxFPS) - endTicks);
		}

		return fps;
	}

	void FPSLimiter::calculateFPS() {
		//uses SDL_GetTicks() to get ms/frames since SDL_Init.
		//ms/frames is then converted into f/s and a circular buffer is used to calculate a rolling
		//average of fps to reduce the effect of outlier ms/f values on the fps value
		static const int NUM_SAMPLES = 100;		//size of our circular buffer
		static float frameTimes[NUM_SAMPLES];	//array of our circular buffer
		static int currentFrame = 0;

		static float prevTicks = SDL_GetTicks();
		float currentTicks;					//how many 'frames' we've had

		currentTicks = SDL_GetTicks();

		frameTime = currentTicks - prevTicks;	//number of ms since last frame
		frameTimes[currentFrame % NUM_SAMPLES] = frameTime;	//insert into circular buffer

		prevTicks = currentTicks;
		currentFrame++;
		//determine how many samples will be in our rolling average 
		int count;

		if (currentFrame < NUM_SAMPLES) {
			count = currentFrame;
		}
		else {								//if buffer is full, there's NUM_SAMPLES valid entries in avg
			count = NUM_SAMPLES;
		}

		//find the average ms/f of the valid entries in our buffer
		float rollingAverage = 0;
		for (int i = 0; i < count; i++) {
			rollingAverage += frameTimes[i];
		}
		rollingAverage /= count;

		//to avoid division by 0
		if (rollingAverage != 0.0f) {
			fps = 1000.0f / rollingAverage;
		}
		else {
			fps = 0.0f;
		}
	}

	
	
	template <typename T>
	Timer<T>::Timer()	
	{
		static_assert(std::is_same<std::micro, T>::value ||
			std::is_same<std::milli, T>::value ||
			std::is_same<std::nano, T>::value,
			"Must pass std::micro, std::nano or std::milli");
	}

	template <typename T>
	void Timer<T>::startTimer()
	{
		m_startTimer = clock::now();
	}

	template <typename T>
	double Timer<T>::endTimer()
	{
		return (clock::now() - m_startTimer).count();
	}

	template class Timer<std::micro>;
	template class Timer<std::milli>;
	template class Timer<std::nano>;
	
}