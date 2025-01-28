#pragma once

#include "GLTexture.h"
#include <string>

namespace Dengine {

	class ImageLoader
	{
	public:
		static GLTexture loadPng(std::string filePath);
	};

}