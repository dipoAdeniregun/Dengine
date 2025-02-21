#pragma once
#include <chrono>

namespace Dengine{
	class FPSLimiter {
	public:
		FPSLimiter();
		void init(float maxFPS);
		void setMaxFPS(float _maxFPS);
		void beginFrame();
		float endFrame();	//return fps

	private:
		float fps;
		float frameTime;
		float maxFPS;

		unsigned int startTicks;

		void calculateFPS();
	};

	template <typename T>
	class Timer {
	public:
		typedef std::chrono::steady_clock clock;
		typedef std::chrono::duration<double, T> duration;

		Timer();
		~Timer() = default;

		void startTimer();
		double endTimer();
		

	private:
		clock::time_point m_startTimer;
	};
}
