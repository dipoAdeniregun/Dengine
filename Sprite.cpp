#include "Sprite.h"
#include "Vertex.h"
#include <cstddef>
#include "ResourceManager.h"

namespace Dengine {

	//constructor
	Sprite::Sprite() {
		vboID = 0;	//a VBO id with id 0 is unassigned so we initialized it to 0
	}

	//destructor
	Sprite::~Sprite() {
		//free our vbo buffer when we are done
		if (vboID) {
			glDeleteBuffers(1, &vboID);
		}
	}

	//initialize the sprite's bottom left coordinates and width and height
	void Sprite::init(float _x, float _y, float _height, float _width, std::string texturePath) {
		x = _x;
		y = _y;
		width = _width;
		height = _height;
		texture = ResourceManager::getTexture(texturePath);

		//assign our sprite a VBO buffer Id if it doesn't already have one
		if (!vboID) {
			glGenBuffers(1, &vboID);
		}

		//initialize the 6 (2 triangles with 3 vertices each with two overlapping vertices) vertices for our sprite
		Vertex vertices[6];

		//first triangle
		vertices[0].setPosition(_x + _width, _y + _height);
		vertices[0].setUV(1.0f, 1.0f);

		vertices[1].setPosition(_x, _y + _height);
		vertices[1].setUV(0.0f, 1.0f);

		vertices[2].setPosition(_x, _y);
		vertices[2].setUV(0.0f, 0.0f);


		//second triangle
		vertices[3].setPosition(_x, _y);
		vertices[3].setUV(0.0f, 0.0f);

		vertices[4].setPosition(_x + _width, _y);
		vertices[4].setUV(1.0f, 0.0f);

		vertices[5].setPosition(_x + _width, _y + _height);
		vertices[5].setUV(1.0f, 1.0f);

		for (int i = 0; i < 6; i++) {
			vertices[i].setColor(255, 0, 255, 255);
		}


		vertices[1].setColor(0, 0, 255, 255);
		vertices[4].setColor(0, 255, 0, 255);



		glBindBuffer(GL_ARRAY_BUFFER, vboID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//draw the sprite to screen
	void Sprite::draw() {
		//bind the texture
		glBindTexture(GL_TEXTURE_2D, texture.id);

		//bind the vbo buffer object
		glBindBuffer(GL_ARRAY_BUFFER, vboID);

		//enable our three attribute arrays for position, color, and uv
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//vertex attribute pointer corresponding to POSITION using interleaved vertex data
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		//vertex attribute pointer corresponding to COLOR using interleaved vertex data
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//vertex attribute pointer corresponding to UV using interleaved vertex data
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

		//draw the 6 vertices to the screen
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//disable all our three attribute arrays 
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		//unbind vbo buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//unbind texture(typically dont want to do this)
		//glBindTexture(GL_TEXTURE0, 0);

	}
}