#include "ImageLoader.h"
#include <vector>
#include "picopng.h"
#include "IOManager.h"
#include "Errors.h"

namespace Dengine {
	//load png files into a GLTexture struct
	GLTexture ImageLoader::loadPng(std::string filePath) {
		GLTexture texture = {};

		//read png file into input buffer
		std::vector<unsigned char> in;
		if (!(IOManager::readFileToBuffer(in, filePath))) {
			perror(filePath.c_str());
			fatalError("COULD NOT LOAD PNG FILE: " + filePath);
		}

		std::vector<unsigned char> out;

		unsigned long width, height;

		//read png into out buffer using decodePNG from th picoPNG library
		int error = decodePNG(out, width, height, &(in[0]), in.size());
		//decodepng returns 0 on success
		if (error != 0) {
			fatalError("DECODEPNG FAILED WITH ERROR CODE: " + error);
		}

		texture.height = height;
		texture.width = width;
		texture.filePath = filePath;



		glGenTextures(1, &(texture.id));
		glBindTexture(GL_TEXTURE_2D, texture.id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, 0);

		return texture;
	}
}