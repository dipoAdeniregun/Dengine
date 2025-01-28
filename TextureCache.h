#pragma once
#include <map>
#include <string>
#include "GLTexture.h"

namespace Dengine {

	class TextureCache
	{
	public:
		TextureCache();
		GLTexture getTexture(std::string filepath);

	private:
		std::map<std::string, GLTexture> textureMap;
	};

}