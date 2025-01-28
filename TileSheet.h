#pragma once
#include "GLTexture.h"
#include <glm/glm.hpp>

namespace Dengine {


	class TileSheet {
	public:
		void init(GLTexture texture, const glm::ivec2& tileDims) {
			m_texture = texture;
			m_dims = tileDims;
		}

		GLTexture m_texture;
		glm::ivec2 m_dims;
		glm::vec4 getUVs(int index, int tileWidth = 1, int tileHeight = 1) {
			//error checking
			if (index < 0) {

			}
			//TODO: switch from y position increasing from bottom-up to the opposite
			int tilePosX = index % m_dims.x ;
			int tilePosY = index / m_dims.x ; 

			//create uv value for dest rect
			glm::vec4 uvs;
			uvs.x = tilePosX / (float)m_dims.x;	//bottom left uv x coord of tile
			uvs.y = tilePosY / (float)m_dims.y; //bottomleft uv y coord of tile
			uvs.z = tileWidth / (float)m_dims.x;		//xwidth of tile compared to whole tilesheet
			uvs.w = tileHeight / (float)m_dims.y;		//ywidth of tile compared to whole tilesheet

			return uvs;
		}

		glm::vec4 getUVs(int tilePosX, int tilePosY, int tileWidth = 1, int tileHeight = 1) {
			//create uv value for dest rect
			glm::vec4 uvs;
			uvs.x = tilePosX / (float)m_dims.x;	//bottom left uv x coord of tile
			uvs.y = tilePosY / (float)m_dims.y; //bottomleft uv y coord of tile
			uvs.z = tileWidth / (float)m_dims.x;		//xwidth of tile compared to whole tilesheet
			uvs.w = tileWidth / (float)m_dims.y;		//ywidth of tile compared to whole tilesheet

			return uvs;
		}

		glm::ivec2 getSize() { return m_dims; }

	private:

	};

}
