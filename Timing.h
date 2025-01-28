#pragma once

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
}
