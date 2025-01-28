#include "Camera2D.h"

namespace Dengine {
	//constructor
	Camera2D::Camera2D() : position(0.0f, 0.0f),
		cameraMatrix(1.0f),
		scale(1.0f),
		needsCamUpdate(true),
		screenWidth(500),
		screenHeight(500),
		orthoMatrix(1.0f)
	{}

	//init
	void Camera2D::init(int screenW, int screenH) {
		screenWidth = screenW;
		screenHeight = screenH;
		orthoMatrix = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight);
	}

	//update the camera
	void Camera2D::update() {
		if (needsCamUpdate) {
			//translate(move) to the right
			glm::vec3 translate(-position.x + screenWidth/2, -position.y + screenHeight/2, 0.0f);
			cameraMatrix = glm::translate(orthoMatrix, translate);

			//scale vector
			glm::vec3 _scale(scale, scale, 0.0f);
			
			cameraMatrix = glm::scale(glm::mat4(1.0f), _scale) * cameraMatrix;

			//update needsCamMatrix
			needsCamUpdate = false;
		}
	}

	//converts the screen coordinates to world coordinates
	glm::vec2 Camera2D::screen2WorldCoords(glm::vec2 screenCoords){
		//invert y
		screenCoords.y = screenHeight - screenCoords.y;
		//convert screen coords from (0,0) top left to (0,0) in the middle
		screenCoords -= glm::vec2(screenWidth / 2, screenHeight / 2);

		//change screen coords by scaling factor
		screenCoords /= scale;

		//move by any translation factor
		screenCoords += position;

		return screenCoords;
	}

	//check if a box is in view from camera. return true if it is
	bool Camera2D::isBoxInView(const glm::vec2& _position, const glm::vec2& dimensions)
	{
		glm::vec2 scaledScreenDim = glm::vec2(screenWidth, screenHeight) / (scale );

		const float MIN_DISTANCE_X = dimensions.x / 2.0f + scaledScreenDim.x / 2.0f;
		const float MIN_DISTANCE_Y = dimensions.y / 2.0f + scaledScreenDim.y / 2.0f;

		glm::vec2 centerPos = _position + (dimensions / 2.0f);
		glm::vec2 centerCameraPos = position;

		glm::vec2 distVec = centerPos - centerCameraPos;

		float xDepth = MIN_DISTANCE_X - abs(distVec.x);
		float yDepth = MIN_DISTANCE_Y - abs(distVec.y);

		//if true, then there is a collission
		if (xDepth > 0 && yDepth > 0) {
			//box is in view return true
			return true;
		}

		return false;
	}
}