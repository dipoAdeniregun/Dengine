#pragma once
#include<GL/glew.h>
#include "GLTexture.h"
#include <string>

namespace Dengine {
	class Sprite
	{
	private:
		int x;
		int y;
		int height;
		int width;
		GLuint vboID;
		GLTexture texture;

	public:
		Sprite();
		~Sprite();

		void init(float _x, float _y, float _height, float _width, std::string texturePath);
		void draw();
	};

}