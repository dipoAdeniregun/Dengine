#include "SpriteBatch.h"
#include<algorithm>

namespace Dengine {

	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint _texture, float _depth, const ColorRGBA8& color)
		:texture(_texture), depth(_depth)
	{
		topLeft.color = color;
		topLeft.setPosition(destRect.x, destRect.y + destRect.w);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x, destRect.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + destRect.z, destRect.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRect.x + destRect.z, destRect.y + destRect.w);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}

	Glyph::Glyph(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint _texture, float _depth, const ColorRGBA8& color, float radAngle)
		:texture(_texture), depth(_depth)
	{
		glm::vec2 halfDims (destRect.z / 2, destRect.w / 2);

		//get points centered at origin
		glm::vec2 tl (-halfDims.x, halfDims.y);
		glm::vec2 tr (halfDims.x, halfDims.y);
		glm::vec2 bl (-halfDims.x, -halfDims.y);
		glm::vec2 br (halfDims.x, -halfDims.y);

		//rotate the points
		tl = rotatePoint(tl, radAngle) + halfDims;
		tr = rotatePoint(tr, radAngle) + halfDims;
		bl = rotatePoint(bl, radAngle) + halfDims;
		br = rotatePoint(br, radAngle) + halfDims;


		topLeft.color = color;
		topLeft.setPosition(destRect.x + tl.x, destRect.y + tl.y);
		topLeft.setUV(uvRect.x, uvRect.y + uvRect.w);

		bottomLeft.color = color;
		bottomLeft.setPosition(destRect.x + bl.x, destRect.y + bl.y);
		bottomLeft.setUV(uvRect.x, uvRect.y);

		bottomRight.color = color;
		bottomRight.setPosition(destRect.x + br.x, destRect.y + br.y);
		bottomRight.setUV(uvRect.x + uvRect.z, uvRect.y);

		topRight.color = color;
		topRight.setPosition(destRect.x + tr.x, destRect.y + tr.y);
		topRight.setUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
	}

	glm::vec2 Glyph::rotatePoint(glm::vec2 point, float radAngle)
	{
		glm::vec2 newV;
		newV.x = (point.x * cos(radAngle)) - (point.y * sin(radAngle));
		newV.y = (point.x * sin(radAngle)) + (point.y * cos(radAngle));

		return newV;
	}

	SpriteBatch::SpriteBatch(): vbo(0), vao(0)
	{
	}

	SpriteBatch::~SpriteBatch(){
	
	}

	void SpriteBatch::begin(GlyphSortType _sortType /*GlyphSortType::TEXTURE*/) {
		sortType = _sortType;
		renderBatches.clear();
		
		glyphs.clear();
	}

	void SpriteBatch::end(){
		//set up pointers to glyphs for fast sorting
		glyphPointers.resize(glyphs.size());
		for (size_t i = 0; i < glyphs.size(); i++)
		{
			glyphPointers[i] = &glyphs[i];
		}

		sortGlyphs();
		createRenderBatches();
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color){
		glyphs.emplace_back(destRect, uvRect, texture, depth, color);
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, float radAngle)
	{
		glyphs.emplace_back(destRect, uvRect, texture, depth, color, radAngle);
	}

	void SpriteBatch::draw(const glm::vec4& destRect, const glm::vec4& uvRect, GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& dir)
	{
		//since angle is measured btw x axis and the dir vector, need a unit vector to get angle of dir
		const glm::vec2 right(1.0f, 0.0f);
		float angle = acos(glm::dot(right, dir));

		if (dir.y < 0) angle = -angle;

		glyphs.emplace_back(destRect, uvRect, texture, depth, color, angle);
	}

	void SpriteBatch::createVertexArray() {
		if(vao == 0) glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		if(vbo == 0) glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		
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

		glBindVertexArray(0);
	}

	void SpriteBatch::createRenderBatches() {
		

		std::vector<Vertex> vertices;
		vertices.resize(6 * glyphPointers.size());

		if (glyphPointers.empty()) {
			return;
		}

		int offset = 0;
		int cv = 0;				//current vertex

		renderBatches.emplace_back(offset, 6, glyphPointers[0]->texture);
		vertices[cv++] = glyphPointers[0]->topLeft;
		vertices[cv++] = glyphPointers[0]->bottomLeft;
		vertices[cv++] = glyphPointers[0]->bottomRight;
		vertices[cv++] = glyphPointers[0]->bottomRight;
		vertices[cv++] = glyphPointers[0]->topRight;
		vertices[cv++] = glyphPointers[0]->topLeft;
		offset += 6;

		for (size_t cg = 1; cg < glyphPointers.size(); cg++) {
			if (glyphPointers[cg]->texture != glyphPointers[cg - 1]->texture)
			{
				renderBatches.emplace_back(offset, 6, glyphPointers[cg]->texture);
			}
			else {
				renderBatches.back().numVertices += 6;
			}

			vertices[cv++] = glyphPointers[cg]->topLeft;
			vertices[cv++] = glyphPointers[cg]->bottomLeft;
			vertices[cv++] = glyphPointers[cg]->bottomRight;
			vertices[cv++] = glyphPointers[cg]->bottomRight;
			vertices[cv++] = glyphPointers[cg]->topRight;
			vertices[cv++] = glyphPointers[cg]->topLeft;
			offset += 6;
		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		//upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
	}

	void SpriteBatch::renderBatch(){
		glBindVertexArray(vao);

		for (size_t i = 0; i < renderBatches.size(); i++)
		{
			glBindTexture(GL_TEXTURE_2D, renderBatches[i].texture);
			glDrawArrays(GL_TRIANGLES, renderBatches[i].offset, renderBatches[i].numVertices);
		}

		glBindVertexArray(0);
	}

	void SpriteBatch::init(){
		createVertexArray();
	}

	void SpriteBatch::dispose()
	{
		if (vao != 0) {
			glDeleteVertexArrays(1, &vao);
			vao = 0;
		}
		if (vbo != 0) {
			glDeleteBuffers(1, &vbo);
			vbo = 0;
		}
	}
	
	void SpriteBatch::sortGlyphs() {
		switch (sortType)
		{
		case(GlyphSortType::FRONT_TO_BACK):
			std::stable_sort(glyphPointers.begin(), glyphPointers.end(), compareBackToFront);
			break;

		case(GlyphSortType::BACK_TO_FRONT):
			std::stable_sort(glyphPointers.begin(), glyphPointers.end(), compareFrontToBack);
			break;

		case(GlyphSortType::TEXTURE):
			std::stable_sort(glyphPointers.begin(), glyphPointers.end(), compareTexture);
			break;

		default:
			break;
		}

		
	}

	bool SpriteBatch::compareFrontToBack(Glyph* a, Glyph* b) {
		return a->depth < b->depth;
	}
	bool SpriteBatch::compareBackToFront(Glyph* a, Glyph* b) {
		return a->depth > b->depth;
	}
	bool SpriteBatch::compareTexture(Glyph* a, Glyph* b) {
		return a->texture < b->texture;
	}
}
