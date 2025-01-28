#pragma once
#include <string>
#include <GL/glew.h>
#include "Errors.h"

namespace Dengine {
	class GLSLProgram
	{
	private:
		GLuint programID;
		GLuint vertexShaderID;
		GLuint fragmentShaderID;
		int numAttributes;
		void compileShader(const char* source, std::string name, GLuint shaderID);


	public:
		GLSLProgram();
		~GLSLProgram();

		void dispose();

		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
		void compileShadersFromSource(const char* vertexSource, const char* fragmentSource,
						const std::string& vertName = "Vertex Shader", const std::string& fragName = "Fragment Shader");

		void linkShaders();
		void addAttribute(const std::string& attributeName);
		void use();
		void unuse();

		GLint getUniformLocation(const std::string& uniformName);
	};

}