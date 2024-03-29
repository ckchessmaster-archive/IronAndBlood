#pragma once

#include <string>
#include <GL/glew.h>

namespace Pixel
{
	//This class handles the compilation, linking, and usage of a GLSL shader program.
	//Reference: http://www.opengl.org/wiki/Shader_Compilation
	class GLSLProgram
	{
	private:
		GLuint _programID;
		GLuint _vertexShaderID;
		GLuint _fragmentShaderID;

		int _numAttributes;

		void compileShader(const std::string& filePath, GLuint id);

	public:
		GLSLProgram();
		~GLSLProgram();

		void compileShaders(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);

		void linkShaders();

		void addAttribute(const std::string& attributeName);

		GLuint getUniformLocation(const std::string& uniformName);

		void use();
		void unuse();
	};
}