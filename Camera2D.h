#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Dengine {
	class Camera2D
	{
	public:
		Camera2D();

		void init(int screenW, int screenH);

		//updates camera
		void update();

		//offset
		void offsetPosition(const glm::vec2& offset) { position += offset; needsCamUpdate = true; }
		void offsetScale(float offset) { scale += offset; if (scale <= 0.001f) scale = 0.001f;  needsCamUpdate = true; }

		//setters 
		void setPosition(const glm::vec2& newPosition) { position = newPosition; needsCamUpdate = true; }
		void setScale(float newScale) { scale = newScale; needsCamUpdate = true; }
		void resizeCamera(int screenW, int screenH) { init(screenW, screenH); needsCamUpdate = true;}

		//getters
		float getScale() { return scale; }
		glm::vec2 getPosition() { return position; }
		glm::mat4 getCameraMatrix() { return cameraMatrix; }
		float getAspectRatio() { return 0; }
		float getAspectRatio() const { return (float)screenWidth / (float)screenHeight; }

		glm::vec2 screen2WorldCoords(glm::vec2 screenCoords);
		bool isBoxInView(const glm::vec2& position, const glm::vec2& dimension);
	private:
		int screenWidth, screenHeight;
		glm::vec2 position;
		glm::mat4 cameraMatrix;
		glm::mat4 orthoMatrix;
		float scale;
		bool needsCamUpdate;

	};

}