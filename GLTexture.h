#pragma once

#include <GL/glew.h>
#include <string>

namespace Dengine {

	struct GLTexture {
		std::string filePath = "";
		GLuint id;
		int width;
		int height;
	};
}