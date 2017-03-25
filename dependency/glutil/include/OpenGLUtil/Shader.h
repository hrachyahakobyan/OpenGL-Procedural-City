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
		static std::shared_ptr<Shader> fromFile(const std::string& vertexPath, const std::string& fragmentPath);
		static std::shared_ptr<Shader> fromCode(const GLchar* vertexCode, const GLchar* fragmentCode);
		static std::shared_ptr<Shader> fromCode(const std::string& vertexCode, const std::string& fragmentCode);
		Shader(const Shader&) = delete;
		Shader& operator=(const Shader&) = delete;
		~Shader();

		inline GLuint getProgram() const { return Program; }
		// Uses the current shader
		inline void use() const { glUseProgram(this->Program);}
	private:
		Shader();
		GLuint Program;
		void initialize(const GLchar* vertexCode, const GLchar* fragmentCode);
	};
}