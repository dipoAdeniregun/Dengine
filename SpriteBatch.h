#pragma once
#include <GL/glew.h>
#include "Vertex.h"
#include <GLM/glm.hpp>
#include <vector>

namespace Dengine {
	enum class GlyphSortType {
		NONE,
		FRONT_TO_BACK,
		BACK_TO_FRONT,
		TEXTURE
	};

	//holds a single sprite
	class Glyph {
	public:
		Glyph(){}
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint _texture, float _depth, const ColorRGBA8& color);

		/* 
			Overloaded glyph constructor rotates the given glyph
			by the supplied angle
		*/
		Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint _texture, float _depth, const ColorRGBA8& color, float radAngle);
			

		GLuint texture;
		float depth;

		Vertex topLeft;
		Vertex bottomLeft;
		Vertex topRight;
		Vertex bottomRight;

	private:
		//rotates a point by a given angle
		glm::vec2 rotatePoint(glm::vec2 point, float radAngle);
	};

	class RenderBatch {
	public:
		RenderBatch(GLuint _offset, GLuint _numVertices, GLuint _texture):offset(_offset), numVertices(_numVertices), texture(_texture){}
		GLuint offset;
		GLuint numVertices;
		GLuint texture;
	};

	class SpriteBatch
	{
	public:
		SpriteBatch();
		~SpriteBatch();

		void begin(GlyphSortType _sortType = GlyphSortType::TEXTURE);
		void end();

		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color);

		/*
			Overloaded draw function rotates the sprite to be drawn by the supplied angle
		*/
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float radAngle);

		/*
			Overloaded draw function rotates the sprite to be drawn by 
			the angle between the given direction vector and the +ve x-axis
		*/
		void draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir);

		void renderBatch();
		
		void init();
		void dispose();
	private:
		void createVertexArray();
		void createRenderBatches();
		void sortGlyphs();

		static bool compareFrontToBack(Glyph* a, Glyph* b);
		static bool compareBackToFront(Glyph* a, Glyph* b);
		static bool compareTexture(Glyph* a, Glyph* b);

		GlyphSortType sortType;
		GLuint vbo;
		GLuint vao;
		
		std::vector<Glyph*> glyphPointers;			///<This is for sorting
		std::vector<Glyph> glyphs;
		std::vector<RenderBatch> renderBatches;
	};

}