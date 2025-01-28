#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

namespace Dengine {
	TextureCache::TextureCache() {}

	GLTexture TextureCache::getTexture(std::string filepath) {

		//look for texture specified at filePath
		auto mit = textureMap.find(filepath);

		//create and insert new texture if not already in map
		if (mit == textureMap.end()) {
			//create new texture
			GLTexture newTexture = ImageLoader::loadPng(filepath);	//error checking handled in loadPng

			//insert into textureMap
			//std::pair<std::string, GLTexture> newPair(filepath, newTexture);
			textureMap.insert(make_pair(filepath, newTexture));
			//std::cout << "loaded cache texture\n";
			return newTexture;
		}

		//std::cout << "Used cache texture\n";
		//GLTexture is the second element in the pair pointed to by map iterator mit
		return mit->second;
	}
}