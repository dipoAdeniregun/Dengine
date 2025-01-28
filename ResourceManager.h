#pragma once
#include "TextureCache.h"
#include <string>

namespace Dengine {
	class ResourceManager
	{
	private:
		static TextureCache textureCache;

	public:
		static GLTexture getTexture(std::string texturePath);
	};

}