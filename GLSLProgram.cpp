#include "GLSLProgram.h"
#include "IOManager.h"

#include <fstream>
#include <vector>

namespace Dengine {

	GLSLProgram::GLSLProgram() : programID{ 0 }, vertexShaderID{ 0 }, fragmentShaderID{ 0 }, numAttributes{ 0 } {}

	GLSLProgram::~GLSLProgram() {

	}

	void GLSLProgram::dispose()
	{
		if (programID) glDeleteProgram(programID);
	}

	void GLSLProgram::compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
		//read shader files into buffers
		std::string vertexSource;
		std::string fragmentSource;

		IOManager::readFileToBuffer(vertexSource, vertexShaderFilePath);
		IOManager::readFileToBuffer(fragmentSource, fragmentShaderFilePath);
		
		compileShadersFromSource(vertexSource.c_str(), fragmentSource.c_str(), vertexShaderFilePath, fragmentShaderFilePath);

	}

	void GLSLProgram::compileShadersFromSource(const char* vertexSource, const char* fragmentSource, 
		const std::string& vertName , const std::string& fragName )
	{
		//create vertex shader
		vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		if (!vertexShaderID) {
			fatalError("COULD NOT CREATE VERTEX SHADER!");
		}

		//create fragment shader
		fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
		if (!fragmentShaderID) {
			fatalError("COULD NOT CREATE FRAGMENT SHADER!");
		}

		programID = glCreateProgram();

		compileShader(vertexSource, vertName, vertexShaderID);
		compileShader(fragmentSource, fragName, fragmentShaderID);
	}

	//link our shaders to the program
	void GLSLProgram::linkShaders() {
		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.
		//programID = glCreateProgram();

		// Attach our shaders to our program
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);

		// Link our program
		glLinkProgram(programID);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(programID, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(programID, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(programID);
			// Don't leak shaders either.
			glDeleteShader(vertexShaderID);
			glDeleteShader(fragmentShaderID);

			// Use the infoLog as you see fit.
			//report error
			perror(&infoLog[0]);
			fatalError("COULD NOT LINK SHADERS!");

		}

		// Always detach shaders after a successful link.
		glDetachShader(programID, vertexShaderID);
		glDetachShader(programID, fragmentShaderID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
	}

	//bind shader attributes
	void GLSLProgram::addAttribute(const std::string& attributeName) {
		glBindAttribLocation(programID, numAttributes++, attributeName.c_str());

	}


	void GLSLProgram::use() {
		glUseProgram(programID);
		for (int i = 0; i < numAttributes; i++)
		{
			glEnableVertexAttribArray(i);
		}
	}

	void GLSLProgram::unuse() {
		glUseProgram(0);
		for (int i = 0; i < numAttributes; i++)
		{
			glDisableVertexAttribArray(i);
		}
	}

	//compile a shader of any type
	void GLSLProgram::compileShader(const char* source, std::string name, GLuint shaderID) {
		//const char* contentPtr = source.c_str();
		glShaderSource(shaderID, 1, &source, nullptr);

		glCompileShader(shaderID);

		//confirm successful compilation
		GLint success;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

		if (success == GL_FALSE) {
			//get length of compile failure log
			GLint maxLength;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			//maxLength includes null character
			std::vector<char> errorLog(maxLength);

			glGetShaderInfoLog(shaderID, maxLength, &maxLength, &errorLog[0]);

			//prevent leaks by deleting shader
			glDeleteShader(shaderID);

			//report error
			perror(&errorLog[0]);
			fatalError("COULD NOT COMPILE SHADER AT " + name);
		}
	}

	//get the GLuint representing a uniform value
	GLint GLSLProgram::getUniformLocation(const std::string& uniformName) {
		GLint location = glGetUniformLocation(programID, uniformName.c_str());
		if (location == GL_INVALID_INDEX) {
			fatalError("UNIFORM " + uniformName + " not FOUND IN SHADER!");
		}

		return location;
	}
}