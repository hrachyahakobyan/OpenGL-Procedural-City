#pragma once
#include "Texture.h"
#include "Resource.h"
#include "Shader.h"

namespace glutil{
	class Skybox
	{
	public:
		static std::shared_ptr<Skybox> make(const std::string& cubemapPath, const std::vector<std::string>& faces);
		static std::shared_ptr<Skybox> make(const std::string& cubemapPath, const std::vector<std::string>& faces, std::shared_ptr<Shader> shader);	
		Skybox(const Skybox&) = delete;
		Skybox& operator=(const Skybox&) = delete;
		~Skybox();
		void draw() const;
		void draw(const glm::mat4& view) const;
		void draw(const glm::mat4& view, const glm::mat4& proj) const;
	private:
		Skybox(const std::string& cubemapPath, const std::vector<std::string>& faces);
		Skybox(const std::string& cubemapPath, const std::vector<std::string>& faces, std::shared_ptr<Shader> shader);
		std::shared_ptr<Texture> cubemap;
		std::shared_ptr<VAO> vao;
		std::shared_ptr<VBO> vbo;
		std::shared_ptr<Shader> shader;
		static const GLfloat positions[];
		static const GLchar vertexCode[];
		static const GLchar fragmentCode[];
		void initialize();
	};
}

