#include "DebugRenderer.h"
//#include <iostream>

const float PI = 3.1415926536f;
namespace{
const char* VERT_SRC = R"(#version 130
//The vertex shader operates on each vertex

//input data from the VBO. Each vertex is 2 floats
in vec2 vertexPosition;
in vec4 vertexColor;

out vec2 fragmentPosition;
out vec4 fragmentColor;

uniform mat4 P;

void main() {
	//Set the x,y position on the screen
	gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
	//the z position is zero since we are in 2D
	gl_Position.z = 0.0;

	//Indicate that the coordinates are normalized
	gl_Position.w = 1.0;

	fragmentPosition = vertexPosition;

	fragmentColor = vertexColor;
})";

const char* FRAG_SRC = R"(#version 130
//The fragment shader operates on each pixel in a given polygon

in vec2 fragmentPosition;
in vec4 fragmentColor;

//This is the 3 component float vector that gets outputted to the screen
//for each pixel.
out vec4 color;

void main() {

    color = fragmentColor;
})";

}

namespace Dengine {
	DebugRenderer::DebugRenderer()
	{
		//Empty
	}

	DebugRenderer::~DebugRenderer()
	{
		dispose();
	}

	void DebugRenderer::init()
	{
		//std::cout << "init\n";
		//shader init
		m_program.compileShadersFromSource(VERT_SRC, FRAG_SRC);
		m_program.addAttribute("vertexPosition");
		m_program.addAttribute("vertexColor");
		m_program.linkShaders();

		//setup buffers
		glGenVertexArrays(1, &m_vao);
		glGenBuffers(1, &m_vbo);
		glGenBuffers(1, &m_ibo);

		glBindVertexArray(m_vao);
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, position));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));

		glBindVertexArray(0);

	}

	void DebugRenderer::end()
	{
		//std::cout << "end\n";
		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		//Orphan the buffer
		glBufferData(GL_ARRAY_BUFFER, m_verts.size() * sizeof(DebugVertex), nullptr, GL_DYNAMIC_DRAW);
		//Upload the data
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_verts.size() * sizeof(DebugVertex), m_verts.data());
		//Unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
		//Orphan the buffer
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
		//Upload the data
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_indices.size() * sizeof(GLuint), m_indices.data());
		//Unbind
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		m_numElements = m_indices.size();
		m_indices.clear();
		m_verts.clear();
	}

	void DebugRenderer::drawLine(const glm::vec2& a, const glm::vec2& b, const ColorRGBA8& color)
	{
		int i = m_verts.size();
		m_verts.resize(m_verts.size() + 2);

		m_verts[i].position = a;
		m_verts[i].color = color;

		m_verts[i+1].position = b;
		m_verts[i+1].color = color;

		m_indices.push_back(i);
		m_indices.push_back(i + 1);
	}

	glm::vec2 rotatePoint(const glm::vec2 point, float radAngle)
	{
		glm::vec2 newV;
		newV.x = (point.x * cos(radAngle)) - (point.y * sin(radAngle));
		newV.y = (point.x * sin(radAngle)) + (point.y * cos(radAngle));

		return newV;
	}

	void DebugRenderer::drawBox(const glm::vec4& destRect, const ColorRGBA8& color, float angle)
	{
		//std::cout << "draw box\n";
		int i = m_verts.size();
		m_verts.resize(m_verts.size() + 4);

		glm::vec2 halfDims(destRect.z / 2.0f, destRect.w / 2.0f);
		glm::vec2 positionOffset(destRect.x, destRect.y);

		//get points centered at origin
		glm::vec2 tl(-halfDims.x, halfDims.y);
		glm::vec2 tr(halfDims.x, halfDims.y);
		glm::vec2 bl(-halfDims.x, -halfDims.y);
		glm::vec2 br(halfDims.x, -halfDims.y);

		//rotate the points
		m_verts[i].position = rotatePoint(tl, angle) + halfDims + positionOffset;
		m_verts[i+1].position = rotatePoint(bl, angle) + halfDims + positionOffset;
		m_verts[i+2].position = rotatePoint(br, angle) + halfDims + positionOffset;
		m_verts[i+3].position = rotatePoint(tr, angle) + halfDims + positionOffset;

		//color the bounding boxes
		for (int j = i; j < i + 4; j++) {
			m_verts[j].color = color;
		}

		//push the indices of the 4 points we're using to draw our 4 lines for the rect
		m_indices.reserve(m_indices.size() + 8);

		m_indices.push_back(i);
		m_indices.push_back(i+1);

		m_indices.push_back(i+1);
		m_indices.push_back(i+2);

		m_indices.push_back(i+2);
		m_indices.push_back(i+3);

		m_indices.push_back(i+3);
		m_indices.push_back(i);
	}

	void DebugRenderer::drawCircle(const glm::vec2& center, const ColorRGBA8& color, float radius)
	{
		//TODO: Get number of Vertexes based on circle size
		int NUM_VERT = 100;

		//set up equally spaced vertices around our bounding circle
		int start = m_verts.size();
		m_verts.resize(m_verts.size() + NUM_VERT);
		for (int i = 0; i < NUM_VERT; i++) {
			float angle = ((float)i / NUM_VERT) * 2.0f * PI;
			m_verts[start + i].position.x = cos(angle) * radius + center.x;
			m_verts[start + i].position.y = sin(angle) * radius + center.y;
			m_verts[start + i].color = color;
		}

		//set the indices for all the line segments in circle
		m_indices.reserve(m_indices.size() + NUM_VERT * 2);
		for (int i = 0; i < NUM_VERT - 1; i++) {
			m_indices.push_back(start + i);
			m_indices.push_back(start + i + 1);
		}
		m_indices.push_back(start + NUM_VERT - 1);
		m_indices.push_back(start);
	}

	void DebugRenderer::render(const glm::mat4& projectionMatrix, float lineWidth)
	{
		//std::cout << "render\n"; 
		m_program.use();

		GLint pUniform = m_program.getUniformLocation("P");
		glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);
		glLineWidth(lineWidth);

		glBindVertexArray(m_vao);
		//draw
		glDrawElements(GL_LINES, m_numElements, GL_UNSIGNED_INT, 0);

		//end draw
		glBindVertexArray(0);
		m_program.unuse();
	}

	void DebugRenderer::dispose()
	{
		if (m_vao) {
			glDeleteVertexArrays(1, &m_vao);
		}

		if (m_vbo) {
			glDeleteBuffers(1, &m_vbo);
		}

		if (m_ibo) {
			glDeleteBuffers(1, &m_ibo);
		}

		m_program.dispose();
	}

}
