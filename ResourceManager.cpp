#include "ResourceManager.h"

namespace Dengine {
	//static variables need to be re-declared
	TextureCache ResourceManager::textureCache;


	GLTexture ResourceManager::getTexture(std::string texturePath) {
		return textureCache.getTexture(texturePath);
	}
}