#pragma once
#include "Resource.h"

namespace glutil{

	/**
	*	@brief Abstracts the shader compilation and linkage
	*	modified from https://learnopengl.com 
	*/
	class Shader 
	{
	public:
		Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
		Shader(const Shader&) = delete;
		Shader() = delete;
		Shader& operator=(const Shader&) = delete;

		inline GLuint getProgram() const { return Program; }
		// Uses the current shader
		void use() const
		{
			glUseProgram(this->Program);
		}
	private:
		GLuint Program;
	};
}